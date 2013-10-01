#include "DnsSD/SRVRecord.h"

#include <ctime>
#include <stdexcept>

#define PROPERTY_SETTER(type, param, fnName)\
SRVRecord::Builder& SRVRecord::Builder::with##fnName(type param) {\
    record->param= param;\
    return *this;\
}

namespace etsai {
namespace dnssd {

using std::runtime_error;

SRVRecord::SRVRecord(int ttl) : port(-1), priority(0), weight(0), ttl(ttl) {
    time_t now;
    time(&now);
    expireTime= now + ttl;
}

const string& SRVRecord::getHostname() const {
    return hostname;
}

uint16_t SRVRecord::getPort() const {
    return port;
}

uint16_t SRVRecord::getPriority() const {
    return priority;
}

uint16_t SRVRecord::getWeight() const {
    return weight;
}

int SRVRecord::getTTL() const {
    return ttl;
}

bool SRVRecord::hasExpired() const {
    time_t now;
    time(&now);

    return expireTime < now;
}

bool SRVRecord::operator <(const SRVRecord& record) const {
    return priority > record.priority || (priority == record.priority && 
            weight < record.weight);
}

SRVRecord::Builder::Builder(int ttl) : record(new SRVRecord(ttl)) {
}

SRVRecord::Builder::~Builder() {
    record.reset();
}

PROPERTY_SETTER(const string&, hostname, Hostname)
PROPERTY_SETTER(uint16_t, port, Port)
PROPERTY_SETTER(uint16_t, priority, Priority)
PROPERTY_SETTER(uint16_t, weight, Weight)

shared_ptr<SRVRecord> SRVRecord::Builder::buildSRVRecord() const {
    if (record->hostname.empty() || record->port < 0) {
        throw runtime_error("Hostname and port must be set for an SRVRecord object");
    }
    return record;
}

}
}
