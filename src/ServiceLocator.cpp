#include "DnsSD/ServiceLocator.h"

#include <resolv.h>
#include <sstream>
#include <stdexcept>

namespace etsai {
namespace dnssd {

using std::runtime_error;
using std::stringstream;

ServiceLocator::ServiceLocator(const string &service, NetProtocol protocol, 
        const string &domain) : service(service), domain(domain), protocol(protocol) {
    for(auto type: {ns_t_txt, ns_t_srv}) {
        query(type);
    }
}

const SRVRecord& ServiceLocator::getNextSrvRecord() {
    throw runtime_error("Not yet implemented");
}
const vector<shared_ptr<SRVRecord>> ServiceLocator::getSrvRecords() {
    throw runtime_error("Not yet implemented");
}
const string& ServiceLocator::getTextValue() {
    return txtRecord->getText();
}

const string& ServiceLocator::getQueryString() const {
    return queryString;
}

void ServiceLocator::query(ns_type type) {
    map<ns_type, ns_msg> queries;

    stringstream queryStream;
    string protocolStr;

    switch (protocol) {
        case UDP:
            protocolStr= "udp";
            break;
        case TCP:
            protocolStr= "tcp";
            break;
        default:
            throw runtime_error("Unrecognized protocol.  See NetProtocol definition in ServiceLocator.h");
    }

    queryStream << "_" << service << "._" << protocolStr << "." << domain;
    queryString= queryStream.str();
    
    unsigned char query_buffer[1024];
    int response= res_query(queryString.c_str(), C_IN, type, query_buffer, sizeof(query_buffer));
    ns_initparse(query_buffer, response, &queries[type]);

    map<ns_type, function<void (const ns_rr &rr)>> callbacks;
    callbacks[ns_t_srv]= [&nsMsg](const ns_rr &rr) -> void {
        char name[1024];
        dn_expand(ns_msg_base(nsMsg), ns_msg_end(nsMsg), ns_rr_rdata(rr) + 6, name, sizeof(name));

        SRVRecord::Builder builder(ns_rr_ttl(rr));
        shared_ptr<SRVRecord> record;

        builder->withHostname(name)-<withPort(ntohs(*(unsigned short*)ns_rr_rdata(rr)))
                ->withPriority(ntohs(*((unsigned short*)ns_rr_rdata(rr) + 1)))
                ->withWeight(ntohs(*((unsigned short*)ns_rr_rdata(rr) + 2)));
        record= builder->buildSRVRecord();
        srvRecords[record->priority].push_back(record);
    };
    callbacks[ns_t_txt]= [&nsMsg](const ns_rr &rr) -> void {
        txtRecord.reset(new TXTRecord(ns_rr_ttl(rr), ns_rr_rdata(rr) + 1));
    };
    for(int x= 0; x < ns_msg_count(nsMsg, ns_s_an); x++) {
        ns_rr rr;
        ns_parserr(&nsMsg, ns_s_an, x, &rr);
        ns_type type= ns_rr_type(rr);
        if (callbacks.count(type)) {
            callbacks[type](rr);
        }
    }
}

}
}
