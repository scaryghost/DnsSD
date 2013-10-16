#include "DnsSD/ServiceLocator.h"
#include "ExceptionImpl.h"

#include <cstdlib>
#include <functional>

#ifndef WIN32
#include <resolv.h>
#else
#include <Rpc.h>
#include <windns.h>
#endif

namespace etsai {
namespace dnssd {

using std::function;

ServiceLocator::ServiceLocator(const string &service, NetProtocol const* protocol, 
        const string &domain) : service(service), domain(domain), protocol(protocol) {
#ifndef WIN32
    for(auto type: {ns_t_txt, ns_t_srv}) {
#else
    WORD types[]= {DNS_TYPE_TEXT, DNS_TYPE_SRV};
    for(auto type: types) {
#endif
        query(type);
    }

    srand(time(NULL));
}

const SRVRecord& ServiceLocator::getNextSrvRecord() {
    int lowestPriority= srvRecords.begin()->first;
    int totalWeight= 0, targetWeight, accumWeight= 0;
    
    if (srvRecords.empty()) {
        throw ExceptionImpl(ERROR_NO_UNUSED_SRV_RECORDS, "All SRV records have been used");
    }
    for(auto it: srvRecords[lowestPriority]) {
        totalWeight+= it->getWeight();
    }
    targetWeight= rand() % (totalWeight + 1);
    auto it= srvRecords[lowestPriority].begin();
    while((accumWeight+= (*it)->getWeight()) < targetWeight) {
        it++;
    }
    usedSrvRecords.push_back(*it);
    srvRecords[lowestPriority].erase(it);
    if (srvRecords[lowestPriority].size() == 0) {
        srvRecords.erase(lowestPriority);
    }
    return *(usedSrvRecords.back());
}

vector<shared_ptr<SRVRecord>> ServiceLocator::getSrvRecords() const {
    vector<shared_ptr<SRVRecord>> records;
    for(auto it: srvRecords) {
        records.insert(records.end(), it.second.begin(), it.second.end());
    }
    records.insert(records.end(), usedSrvRecords.begin(), usedSrvRecords.end());
    return records;
}
const string& ServiceLocator::getTextValue() const {
    return txtRecord->getText();
}

const string& ServiceLocator::getQueryString() const {
    return queryString;
}

#ifndef WIN32
void ServiceLocator::query(ns_type type) {
    if (type != ns_t_txt && type != ns_t_srv) {
        throw ExceptionImpl(INVALID_NS_TYPE, "Only TXT and SRV records supported");
    }

    queryString= "_" + service + "._" + protocol->getName() + "." + domain;
    
    ns_msg nsMsg;
    unsigned char query_buffer[1024];
    int response= res_query(queryString.c_str(), C_IN, type, query_buffer, sizeof(query_buffer));
    if (response < 0) {
        ExceptionImpl::Builder builder;

        builder.withMessage("Error retriving ") << (type == ns_t_txt ? "TXT" : "SRV") 
                << " records for: " << queryString;
        throw ExceptionImpl(ERROR_DNSSD_QUERY, builder.buildMessage()); 
    }
    ns_initparse(query_buffer, response, &nsMsg);

    map<ns_type, function<void (const ns_rr &rr)>> callbacks;
    callbacks[ns_t_srv]= [&nsMsg, this](const ns_rr &rr) -> void {
        char name[1024];
        dn_expand(ns_msg_base(nsMsg), ns_msg_end(nsMsg), ns_rr_rdata(rr) + 6, name, sizeof(name));

        SRVRecord::Builder builder(ns_rr_ttl(rr));
        shared_ptr<SRVRecord> record;

        auto data= (unsigned short*)ns_rr_rdata(rr);
        builder.withHostname(name).withPriority(ntohs(*data))
                .withWeight(ntohs(data[1])).withPort(ntohs(data[2]));
        record= builder.buildSRVRecord();
        srvRecords[record->getPriority()].insert(record);
    };
    callbacks[ns_t_txt]= [&nsMsg, this](const ns_rr &rr) -> void {
        txtRecord.reset(new TXTRecord(ns_rr_ttl(rr), (const char *)(ns_rr_rdata(rr) + 1)));
    };
    for(int x= 0; x < ns_msg_count(nsMsg, ns_s_an); x++) {
        ns_rr rr;
        ns_parserr(&nsMsg, ns_s_an, x, &rr);
        ns_type type= ns_rr_type(rr);
        if (callbacks.count(type)) {
            callbacks[type](rr);
        }
    }
}
#else
void ServiceLocator::query(WORD type) {
    if (type != DNS_TYPE_TEXT && type != DNS_TYPE_SRV) {
        throw ExceptionImpl(INVALID_NS_TYPE, "Only TXT and SRV records supported");
    }

    queryString= "_" + service + "._" + protocol->getName() + "." + domain;

    PDNS_RECORD pDnsRecord;
    if (DnsQuery_A(queryString.c_str(), type, DNS_QUERY_BYPASS_CACHE, NULL, &pDnsRecord, NULL)) {
        ExceptionImpl::Builder builder;

        builder.withMessage("Error retriving ") << (type == DNS_TYPE_TEXT ? "TXT" : "SRV") 
                << " records for: " << queryString;
        throw ExceptionImpl(ERROR_DNSSD_QUERY, builder.buildMessage()); 
    }

    map<WORD, function<void (PDNS_RECORD it)>> callbacks;
    callbacks[DNS_TYPE_SRV]= [this](PDNS_RECORD it) -> void {
        SRVRecord::Builder builder(it->dwTtl);
        shared_ptr<SRVRecord> record;

        builder.withHostname((char *)RPC_CSTR(it->Data.SRV.pNameTarget)).withPriority(it->Data.SRV.wPriority)
                .withWeight(it->Data.SRV.wWeight).withPort(it->Data.SRV.wPort);
        record= builder.buildSRVRecord();
        srvRecords[record->getPriority()].insert(record);
    };
    callbacks[DNS_TYPE_TEXT]= [this](PDNS_RECORD it) -> void {
        string text;
        for(DWORD i= 0; i < it->Data.TXT.dwStringCount; i++) {
            text+= (char *)RPC_CSTR(it->Data.TXT.pStringArray[i]);
        }
        txtRecord.reset(new TXTRecord(it->dwTtl, text));
    };

     for(auto it= pDnsRecord; it != NULL; it= it->pNext) {
        if (callbacks.count(it->wType)) {
            callbacks[it->wType](it);
        }
    }
    DnsRecordListFree(pDnsRecord, DnsFreeRecordListDeep);
}
#endif

}
}
