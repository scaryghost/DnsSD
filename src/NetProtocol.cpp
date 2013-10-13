#include "DnsSD/NetProtocol.h"
#include "ExceptionImpl.h"

#include <algorithm>
#include <cctype>
#include <functional>
#include <stdexcept>
#include <unordered_map>

namespace etsai {
namespace dnssd {

using std::ptr_fun;
using std::runtime_error;
using std::tolower;
using std::transform;
using std::unordered_map;

const NetProtocol NetProtocol::UDP("udp"), NetProtocol::TCP("tcp");
static const unordered_map<string, NetProtocol const*> netprotoLookup= {{"udp", &NetProtocol::UDP}, {"tcp", &NetProtocol::TCP}};

NetProtocol const* NetProtocol::getNetProtocol(string name) {
    transform(name.begin(), name.end(), name.begin(), ptr_fun<int, int>(tolower));

    if (!netprotoLookup.count(name)) {
        throw ExceptionImpl(INVALID_NET_PROTOCOL, "Invalid NetProtocol name: " + name);
    }
    return netprotoLookup.find(name)->second;
}

NetProtocol::NetProtocol(const string& name) : name(name) {
}

const string& NetProtocol::getName() const {
    return name;
}

}
}
