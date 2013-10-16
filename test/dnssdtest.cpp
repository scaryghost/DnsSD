#include "DnsSD/ServiceLocator.h"
#include "DnsSD/Exception.h"

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#ifdef WIN32
#pragma comment(lib, "DnsSD.lib")
#endif

using namespace std;
using namespace etsai::dnssd;

int main(int argc, char **argv) {
    unordered_map<string, int> counts;
    ServiceLocator *locator= NULL;
    
    try {
        locator= new ServiceLocator(argv[1], NetProtocol::getNetProtocol(argv[2]), argv[3]);

        for(auto it: locator->getSrvRecords()) {
        cout << "{hostname: " << it->getHostname() << ", port: " << it->getPort() << 
                ", priority: " << it->getPriority() << ", weight: " << it->getWeight() << "}" << endl;
        }

        while(true) {
            auto record= locator->getNextSrvRecord();

            stringstream address;
            address << record.getHostname() << ":" << record.getPort();
            if (!counts.count(address.str())) {
                counts[address.str()]= 0;
            }
            counts[address.str()]++;
        }
    } catch (Exception &ex) {
        cout << ex.getMessage() << endl;
    }

    for(auto it: counts) {
        cout << it.first << "- " << it.second << endl;
    }
    
    return 0;
}
