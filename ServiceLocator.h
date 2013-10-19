#ifndef ETSAI_DNSSD_SERVICE_LOCATOR_H
#define ETSAI_DNSSD_SERVICE_LOCATOR_H

#include "NetProtocol.h"
#include "SRVRecord.h"
#include "TXTRecord.h"

#include <memory>
#include <map>
#include <set>
#include <vector>

#ifndef WIN32
#include <arpa/nameser.h>
#else
#include <windows.h>
#endif

namespace etsai {
namespace dnssd {

using std::shared_ptr;
using std::map;
using std::set;
using std::vector;

class ServiceLocator {
private:
    struct SPComparator {
        bool operator()(const shared_ptr<SRVRecord> &left, const shared_ptr<SRVRecord> &right) const {
            return (*left) < (*right);
        }
    };

public:
    class SRVRecordIterator {
    public:
        SRVRecordIterator(const map<int, set<shared_ptr<SRVRecord>, SPComparator>>& copy);
        bool hashNext() const;
        const SRVRecord& next();
    
    private:

        map<int, set<shared_ptr<SRVRecord>, SPComparator>> srvRecords;
    };

    ServiceLocator(const string &service, NetProtocol const* protocol, const string &domain);

    SRVRecordIterator getSrvRecords() const;
    const string& getTextValue() const;
    const string& getQueryString() const;

private:
#ifndef WIN32
    void query(ns_type type);
#else
    void query(WORD type);
#endif

    string service, domain;
    NetProtocol const* protocol;
    string queryString;
    
    shared_ptr<TXTRecord> txtRecord;
    map<int, set<shared_ptr<SRVRecord>, SPComparator>> srvRecords;
};  //class ServiceLocator

}   //namespace dnssd
}   //namespace etsai

#endif
