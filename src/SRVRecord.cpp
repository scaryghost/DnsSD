#include "DnsSD/SRVRecord.h"
#include "ExceptionImpl.h"

#include <ctime>
#include <initializer_list>
#include <numeric>

#define PROPERTY_GETTER(type, variable, fnName)\
type SRVRecord::get##fnName() const {\
    return variable;\
}

#define PROPERTY_SETTER(type, param, fnName)\
SRVRecord::Builder& SRVRecord::Builder::with##fnName(type param) {\
    record->param= param;\
    return *this;\
}

#define COMPARE(param) (param > record.param ? 1 : (param == record.param ? 0 : -1))

namespace etsai {
namespace dnssd {

using std::initializer_list;
using std::inner_product;

SRVRecord::SRVRecord(int ttl) : RecordType(ttl), port(-1), priority(0), weight(0) {
}

PROPERTY_GETTER(const string&, hostname, Hostname)
PROPERTY_GETTER(uint16_t, port, Port)
PROPERTY_GETTER(uint16_t, priority, Priority)
PROPERTY_GETTER(uint16_t, weight, Weight)

bool SRVRecord::operator <(const SRVRecord& record) const {
    auto values= {COMPARE(priority), COMPARE(weight), COMPARE(hostname), COMPARE(port)};
    auto weights= {8, 4, 2, 1};

    return inner_product(values.begin(), values.end(), weights.begin(), 0) < 0;
}

string SRVRecord::toString() const {
    stringstream srvStream;

    srvStream << "{priority: " << priority << ", weight: " << weight << 
            ", port: " << port << ", hostname: " << hostname << "}";
    return srvStream.str();
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
        throw ExceptionImpl(ERROR_SRV_BUILDER, "Hostname and port must be set to build an SRVRecord object");
    }
    return record;
}

}
}
