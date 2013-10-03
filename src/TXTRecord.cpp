#include "DnsSD/TXTRecord.h"

namespace etsai {
namespace dnssd {

TXTRecord::TXTRecord(int ttl, const string& text) : RecordType(ttl), text(text) {
}

const string& TXTRecord::getText() const {
    return text;
}

}
}
