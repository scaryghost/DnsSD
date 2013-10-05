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
    
    for(auto type: {ns_t_txt, ns_t_srv}) {
        unsigned char query_buffer[1024];
        int response= res_query(queryString.c_str(), C_IN, type, query_buffer, sizeof(query_buffer));
        ns_initparse(query_buffer, response, &queries[type]);
    }
}

const SRVRecord& ServiceLocator::getNextSrvRecord() {
    throw runtime_error("Not yet implemented");
}
const vector<shared_ptr<SRVRecord>> ServiceLocator::getSrvRecords() {
    throw runtime_error("Not yet implemented");
}
const string& ServiceLocator::getTextValue() {
    throw runtime_error("Not yet implemented");
}

const string& ServiceLocator::getQueryString() const {
    throw runtime_error("Not yet implemented");
}

}
}
