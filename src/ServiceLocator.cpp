#include "DnsSD/ServiceLocator.h"

#ifdef WIN32
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Dnsapi.lib")
#endif

ServiceLocator::ServiceLocator(const string &service, NetProtocol protocol, 
        const string &domain) : service(service), protocol(protocol), domain(domain) {
}
