#ifndef ETSAI_DNSSD_SERVICE_LOCATOR_H
#define ETSAI_DNSSD_SERVICE_LOCATOR_H

#include "SRVRecord.h"

#include <memory>
#include <map>
#include <vector>

#include <arpa/nameser.h>

namespace etsai {
namespace dnssd {

using std::shared_ptr;
using std::map;
using std::vector;

enum NetProtocol {
    TCP,
    UDP
};

class ServiceLocator {
public:
    ServiceLocator(const string &service, NetProtocol protocol, const string &domain);

    const SRVRecord& getNextSrvRecord();
    const vector<shared_ptr<SRVRecord>> getSrvRecords();
    const string& getTextValue();

    const string& getQueryString() const;

private:
    string service, domain;
    NetProtocol protocol;
    string queryString;
    
    map<ns_type, ns_msg> queries;
};  //class ServiceLocator

}   //namespace dnssd
}   //namespace etsai

#endif
