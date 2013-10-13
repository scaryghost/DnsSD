#include "DnsSD/src/ExceptionImpl.h"

namespace etsai {
namespace dnssd {

ExceptionImpl::Builder::Builder() {
}

stringstream& ExceptionImpl::Builder::withMessage(const string& base) {
    msgStream << base;
    return msgStream;
}

string ExceptionImpl::Builder::buildMessage() const {
    return msgStream.str();
}

ExceptionImpl::ExceptionImpl(int status, const string& msg) : exceptionInfo(make_pair(status, msg)) {
}

int ExceptionImpl::getStatus() const {
    return exceptionInfo.first;
}

const char* ExceptionImpl::getMessage() const {
    return exceptionInfo.second.c_str();
}


}
}
