#ifndef ETSAI_DNSSD_SRVRECORD_H
#define ETSAI_DNSSD_SRVRECORD_H

#include <cstdint>
#include <ctime>
#include <memory>
#include <string>

namespace etsai {
namespace dnssd {

using std::shared_ptr;
using std::string;

/**
 * Represents an SRV resource record queried from an SRV lookup.  The specifications for this class 
 * are taken from: http://dns-sd.org/ and http://www.faqs.org/rfcs/rfc2782.html
 * @author etsai
 */
class SRVRecord {
    /**
     * Auxiliary class to assist with building an SRV record.  All SRV records 
     * must be created using this builder.
     * @author etsai
     */
    class Builder {
    public:
        /**
         * Builder construtor that initializes an SRV record with the specified ttl.  A ttl value 
         * of 0 signifies the SRV record should not be cached.
         * @param   ttl     Optional parameter that sets the SRV record's ttl value (in seconds).  
         *                  Defaults to 0
         */
        Builder(int ttl= 0);
        /**
         * Releases the shared pointer refernce to the created SRVRecord
         */
        ~Builder();

        /**
         * Sets the hostname of the SRV record
         * @param   hostname    Hostname for the SRV record
         * @return  Reference to the builder
         */
        Builder& withHostname(const string& hostname);
        /**
         * Sets the port number of the SRV record
         * @param   port        Port number for the SRV record
         * @return  Reference to the builder
         */
        Builder& withPort(uint16_t port);
        /**
         * Sets the priority value of the SRV record
         * @param   priority    Priority value for the SRV record
         * @return  Reference to the builder
         */
        Builder& withPriority(uint16_t priority);
        /**
         * Sets the weight of the SRV record
         * @param   weight      Weight for the SRV record
         * @return  Reference to the builder
         */
        Builder& withWeight(uint16_t weight);

        /**
         * Build an SRVRecord with the given parameters
         * @return  Pointer to the created record
         * @throw   std::runtime_exception  If hostname and port values were not set
         */
        shared_ptr<SRVRecord> buildSRVRecord() const;

    private:
        shared_ptr<SRVRecord> record;   ///< Pointer to the created SRV record
    };

    /**
     * Get the hostname of the SRV record
     * @return  Record's hostname
     */
    const string& getHostname() const;
    /**
     * Get the port number of the SRV record
     * @return  Record's port number
     */
    uint16_t getPort() const;
    /**
     * Get the priority value of the SRV record
     * @return  Record's priority value
     */
    uint16_t getPriority() const;
    /**
     * Get the weight of the SRV record
     * @return  Record's weight
     */
    uint16_t getWeight() const;
    /**
     * Get the TTL of the SRV record
     * @return  Record's TTL
     */
    int getTTL() const;
    /**
     * Determines if the SRV Record has expired based on its creation time and TTL value.
     * @return  True if the record has expired
     */
    bool hasExpired() const;

    /**
     * Determines if the caller SRV record should be used before the parameter record.
     * The comparison is determined first by evaulating this.priority < record.priority, and 
     * if both priorities are equal, then evaluating this.weight > record.weight as the tiebreaker.
     * @return  True if this record should be used before parameter record
     */
    bool operator <(const SRVRecord& record) const;

private:
    /**
     * Constructs an SRVRecord with a specified ttl value
     * @param   ttl     How long the record should be cached, in seconds
     */
    SRVRecord(int ttl);

    uint16_t port, priority, weight;
    int ttl;
    time_t expireTime;
    string hostname;
};  //class DnsSD

}   //namespace dnssd
}   //namespace etsai

#endif
