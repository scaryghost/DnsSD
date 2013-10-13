#ifndef ETSAI_DNSSD_EXCEPTION_IMPL_H
#define ETSAI_DNSSD_EXCEPTION_IMPL_H

#include "DnsSD/Exception.h"

#include <sstream>
#include <string>

namespace etsai {
namespace dnssd {

using std::pair;
using std::string;
using std::stringstream;

class ExceptionImpl : public Exception {
public:
    class Builder {
    public:
        Builder();

        stringstream& withMessage(const string& base);
        string buildMessage() const;

    private:
        stringstream msgStream;
    };

    ExceptionImpl(int status, const string& msg);

    virtual int getStatus() const;
    virtual const char* getMessage() const;

private:
    pair<int, string> exceptionInfo;
};

}
}

#endif
