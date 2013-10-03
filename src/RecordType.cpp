#include "DnsSD/RecordType.h"

namespace etsai {
namespace dnssd {

RecordType::RecordType(int ttl) : ttl(ttl) {
    time_t now;
    time(&now);
    expireTime= now + ttl;
}

int RecordType::getTTL() const {
    return ttl;
}

bool RecordType::hasExpired() const {
    time_t now;
    time(&now);

    return expireTime < now;
}

}
}
