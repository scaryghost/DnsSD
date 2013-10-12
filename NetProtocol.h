#ifndef ETSAI_DNSSD_NET_PROTOCOL_H
#define ETSAI_DNSSD_NET_PROTOCOL_H

#include <string>

namespace etsai {
namespace dnssd {

using std::string;

class NetProtocol {
public:
    static const NetProtocol UDP, TCP;
    static NetProtocol const* getNetProtocol(string name);

    const string& getName() const;
private:
    NetProtocol(const string& name);

    string name;
};

}
}

#endif
