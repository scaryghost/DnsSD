#ifndef ETSAI_DNSSD_SERVICE_LOCATOR_H
#define ETSAI_DNSSD_SERVICE_LOCATOR_H

#include "SRVRecord.h"
#include "TXTRecord.h"

#include <memory>
#include <map>
#include <set>
#include <vector>

#include <arpa/nameser.h>

namespace etsai {
namespace dnssd {

using std::shared_ptr;
using std::map;
using std::set;
using std::vector;

enum NetProtocol {
    TCP,
    UDP
};

class ServiceLocator {
public:
    ServiceLocator(const string &service, NetProtocol protocol, const string &domain);

    const SRVRecord& getNextSrvRecord();
    const map<int, set<shared_ptr<SRVRecord>, SPComparator>>& getSrvRecords();
    const string& getTextValue() const;

    const string& getQueryString() const;

private:
    struct SPComparator {
        bool operator()(const shared_ptr<SRVRecord> &left, const shared_ptr<SRVRecord> &right) {
            return (*left) < (*right);
        }
    };
    void query(ns_type type);

    string service, domain;
    NetProtocol protocol;
    string queryString;
    
    shared_ptr<TXTRecord> txtRecord;
    map<int, set<shared_ptr<SRVRecord>, SPComparator>> srvRecords;
    vector<shared_ptr<SRVRecord>> usedSrvRecords;
};  //class ServiceLocator

}   //namespace dnssd
}   //namespace etsai

#endif
