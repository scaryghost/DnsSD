#include "DnsSD/NetProtocol.h"
#include "ExceptionImpl.h"

#include <algorithm>
#include <cctype>
#include <initializer_list>
#include <functional>
#include <unordered_map>

namespace etsai {
namespace dnssd {

using std::initializer_list;
using std::ptr_fun;
using std::tolower;
using std::transform;
using std::unordered_map;

const NetProtocol NetProtocol::UDP("udp"), NetProtocol::TCP("tcp");
#ifndef WIN32
static const unordered_map<string, NetProtocol const*> netprotoLookup= {{"udp", &NetProtocol::UDP}, {"tcp", &NetProtocol::TCP}};
#else
struct NetProtocolInfo {
    string name;
    NetProtocol const* ptr;
};

NetProtocolInfo netProtoLookup[]= {{"udp", &NetProtocol::UDP}, {"tcp", &NetProtocol::TCP}};
#endif

NetProtocol const* NetProtocol::getNetProtocol(string name) {
    transform(name.begin(), name.end(), name.begin(), ptr_fun<int, int>(tolower));

#ifndef WIN32
    if (!netprotoLookup.count(name)) {
        throw ExceptionImpl(INVALID_NET_PROTOCOL, "Invalid NetProtocol name: " + name);
    }
    return netprotoLookup.find(name)->second;
#else
    NetProtocol const* ptr= NULL;
    for(auto it: netProtoLookup) {
        if (it.name == name) {
            ptr= it.ptr;
            break;
        }
    }

    if (ptr == NULL) {
        throw ExceptionImpl(INVALID_NET_PROTOCOL, "Invalid NetProtocol name: " + name);
    }
    return ptr;
#endif
}

NetProtocol::NetProtocol(const string& name) : name(name) {
}

const string& NetProtocol::getName() const {
    return name;
}

}
}
