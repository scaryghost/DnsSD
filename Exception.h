#ifndef ETSAI_DNSSD_EXCEPTION_H
#define ETSAI_DNSSD_EXCEPTION_H

namespace etsai {
namespace dnssd {

const int INVALID_NET_PROTOCOL= 1;
const int INVALID_NS_TYPE= 2;
const int ERROR_DNSSD_QUERY= 4;
const int ERROR_SRV_BUILDER= 8;
const int ERROR_NO_UNUSED_SRV_RECORDS= 16;

class Exception {
public:
    virtual int getStatus() const= 0;
    virtual const char* getMessage() const= 0;
};

}
}

#endif
