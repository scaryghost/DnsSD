#include "DnsSD/ServiceLocator.h"
#include "DnsSD/Exception.h"

#include <iostream>

#ifdef WIN32
#pragma comment(lib, "DnsSD.lib")
#endif

using namespace std;
using namespace etsai::dnssd;

int main(int argc, char **argv) {
    if (argc < 4) {
        cerr << "usage: " << argv[0] << " [service] [udp|tcp] [domain]" << endl;
        exit(1);
    }

    try {
        ServiceLocator locator(argv[1], NetProtocol::getNetProtocol(argv[2]), argv[3]);
        auto records= locator.getSrvRecords();

        cout << "TXT Record: " << locator.getTextValue() << endl;
        while(records.hashNext()) {
            auto it= records.next();
            cout << "SRV Record: {hostname: " << it.getHostname() << ", port: " << it.getPort() << 
                    ", priority: " << it.getPriority() << ", weight: " << it.getWeight() << "}" << endl;
        }

    } catch (Exception &ex) {
        cout << ex.getMessage() << endl;
    }

    return 0;
}
