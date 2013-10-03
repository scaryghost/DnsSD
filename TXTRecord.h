#ifndef ETSAI_DNSSD_TXT_RECORD_H
#define ETSAI_DNSSD_TXT_RECORD_H

#include "DnsSD/RecordType.h"
#include <string>

namespace etsai {
namespace dnssd {

using std::string;

class TXTRecord : public RecordType {
public:
    TXTRecord(int ttl, const string& text);

    const string& getText() const;

private:
    string text;
};

}
}

#endif
