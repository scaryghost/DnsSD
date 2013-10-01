#ifndef ETSAI_DNSSD_SRVRECORD_H
#define ETSAI_DNSSD_SRVRECORD_H

#include <cstdint>
#include <ctime>
#include <memory>
#include <string>

namespace etsai {
namespace dnssd {

using std::shared_ptr;
using std::string;

class SRVRecord {
    class Builder {
    public:
        Builder(int ttl);
        ~Builder();

        Builder& withHostname(const string& hostname);
        Builder& withPort(uint16_t port);
        Builder& withPriority(uint16_t priority);
        Builder& withWeight(uint16_t weight);

        shared_ptr<SRVRecord> buildSRVRecord() const;

    private:
        shared_ptr<SRVRecord> record;
    };

    const string& getHostname() const;
    uint16_t getPort() const;
    uint16_t getPriority() const;
    uint16_t getWeight() const;
    int getTTL() const;
    bool hasExpired() const;

    bool operator <(const SRVRecord& record) const;

private:
    SRVRecord(int ttl);

    uint16_t port, priority, weight;
    int ttl;
    time_t expireTime;
    string hostname;
};  //class DnsSD

}   //namespace dnssd
}   //namespace etsai

#endif
