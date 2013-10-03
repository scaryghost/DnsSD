#ifndef ETSAI_DNSSD_RECORD_TYPE_H
#define ETSAI_DNSSD_RECORD_TYPE_H

#include <ctime>

namespace etsai {
namespace dnssd {

class RecordType {
public:
    RecordType(int ttl);
    /**
     * Determines if the record has expired based on its creation time and TTL value.
     * @return  True if the record has expired
     */
    bool hasExpired() const;
    /**
     * Get the TTL of the record
     * @return  Record's TTL
     */
    int getTTL() const;

private:
    time_t expireTime;
    int ttl;
};

}   //namespace dnssd
}   //namespace etsai

#endif
