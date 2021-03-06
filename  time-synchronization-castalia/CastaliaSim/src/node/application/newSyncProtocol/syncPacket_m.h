//
// Generated file, do not edit! Created by opp_msgc 4.2 from src/node/application/newSyncProtocol/syncPacket.msg.
//

#ifndef _SYNCPACKET_M_H_
#define _SYNCPACKET_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0402
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif

// cplusplus {{
#include "ApplicationPacket_m.h"
// }}



/**
 * Class generated from <tt>src/node/application/newSyncProtocol/syncPacket.msg</tt> by opp_msgc.
 * <pre>
 * packet syncPacket  extends ApplicationPacket {
 *     
 *     simtime_t trsTime;
 * 	simtime_t rcvTime;
 * 	simtime_t Emax;
 * 	double maxDrift;
 * 	simtime_t Tsync;  	
 * }
 * </pre>
 */
class syncPacket : public ::ApplicationPacket
{
  protected:
    simtime_t trsTime_var;
    simtime_t rcvTime_var;
    simtime_t Emax_var;
    double maxDrift_var;
    simtime_t Tsync_var;

  private:
    void copy(const syncPacket& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const syncPacket&);

  public:
    syncPacket(const char *name=NULL, int kind=0);
    syncPacket(const syncPacket& other);
    virtual ~syncPacket();
    syncPacket& operator=(const syncPacket& other);
    virtual syncPacket *dup() const {return new syncPacket(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual simtime_t getTrsTime() const;
    virtual void setTrsTime(simtime_t trsTime);
    virtual simtime_t getRcvTime() const;
    virtual void setRcvTime(simtime_t rcvTime);
    virtual simtime_t getEmax() const;
    virtual void setEmax(simtime_t Emax);
    virtual double getMaxDrift() const;
    virtual void setMaxDrift(double maxDrift);
    virtual simtime_t getTsync() const;
    virtual void setTsync(simtime_t Tsync);
};

inline void doPacking(cCommBuffer *b, syncPacket& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, syncPacket& obj) {obj.parsimUnpack(b);}


#endif // _SYNCPACKET_M_H_
