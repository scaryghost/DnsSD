#include "DnsSD/ServiceLocator.h"

#include <cstdlib>
#include <functional>
#include <resolv.h>
#include <sstream>
#include <stdexcept>

namespace etsai {
namespace dnssd {

using std::function;
using std::runtime_error;
using std::stringstream;

ServiceLocator::ServiceLocator(const string &service, NetProtocol const* protocol, 
        const string &domain) : service(service), domain(domain), protocol(protocol) {
    for(auto type: {ns_t_txt, ns_t_srv}) {
        query(type);
    }

    srand(time(NULL));
}

const SRVRecord& ServiceLocator::getNextSrvRecord() {
    int lowestPriority= srvRecords.begin()->first;
    int totalWeight= 0, targetWeight, accumWeight= 0;
    
    for(auto it: srvRecords[lowestPriority]) {
        totalWeight+= it->getWeight();
    }
    targetWeight= rand() % (totalWeight + 1);
    auto it= srvRecords[lowestPriority].begin();
    while((accumWeight+= (*it)->getWeight()) < targetWeight) {
        it++;
    }
    usedSrvRecords.push_back(*it);
    srvRecords[lowestPriority].erase(it);
    if (srvRecords[lowestPriority].size() == 0) {
        srvRecords.erase(lowestPriority);
    }
    return *(usedSrvRecords.back());
}

vector<shared_ptr<SRVRecord>> ServiceLocator::getSrvRecords() const {
    vector<shared_ptr<SRVRecord>> records;
    for(auto it: srvRecords) {
        records.insert(records.end(), it.second.begin(), it.second.end());
    }
    return records;
}
const string& ServiceLocator::getTextValue() const {
    return txtRecord->getText();
}

const string& ServiceLocator::getQueryString() const {
    return queryString;
}

void ServiceLocator::query(ns_type type) {
    stringstream queryStream;

    queryStream << "_" << service << "._" << protocol->getName() << "." << domain;
    queryString= queryStream.str();
    
    ns_msg nsMsg;
    unsigned char query_buffer[1024];
    int response= res_query(queryString.c_str(), C_IN, type, query_buffer, sizeof(query_buffer));
    if (response < 0) {
        stringstream msg;

        msg << "Error retriving ";
        switch (type) {
            case ns_t_txt:
                msg << "TXT";
                break;
            case ns_t_srv:
                msg << "SRV";
                break;
            default:
                throw runtime_error("Only TXT and SRV records supported");
        }
        msg << " records for: " << queryString;
        throw runtime_error(msg.str()); 
    }
    ns_initparse(query_buffer, response, &nsMsg);

    map<ns_type, function<void (const ns_rr &rr)>> callbacks;
    callbacks[ns_t_srv]= [&nsMsg, this](const ns_rr &rr) -> void {
        char name[1024];
        dn_expand(ns_msg_base(nsMsg), ns_msg_end(nsMsg), ns_rr_rdata(rr) + 6, name, sizeof(name));

        SRVRecord::Builder builder(ns_rr_ttl(rr));
        shared_ptr<SRVRecord> record;

        auto data= (unsigned short*)ns_rr_rdata(rr);
        builder.withHostname(name).withPriority(ntohs(*data))
                .withWeight(ntohs(data[1])).withPort(ntohs(data[2]));
        record= builder.buildSRVRecord();
        srvRecords[record->getPriority()].insert(record);
    };
    callbacks[ns_t_txt]= [&nsMsg, this](const ns_rr &rr) -> void {
        txtRecord.reset(new TXTRecord(ns_rr_ttl(rr), (const char *)(ns_rr_rdata(rr) + 1)));
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
