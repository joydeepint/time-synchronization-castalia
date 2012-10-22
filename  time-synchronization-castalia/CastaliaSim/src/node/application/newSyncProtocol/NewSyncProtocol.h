#ifndef _NEWSYNCPROTOCOL_H_
#define _NEWSYNCPROTOCOL_H_

#include "VirtualApplication.h"
#include "syncPacket_m.h"


using namespace std;

struct offset_info {
    int sn;
    simtime_t time;
    simtime_t offset;
};

enum Timers {
    SEND_SYNC = 1,      // Timer to send sync messages (base station)
    RCV_SYNC = 2,       // Timer to wake up and receive sync messages (node)
    SEND_INFO = 3,      // Timer to send initial information to the base station
    DRIFT_UPDATE = 40
};

class NewSyncProtocol: public VirtualApplication {
private:
    bool isBS;              // To identify the base station
    bool canSleep;
    simtime_t Tsync;        // Synchronization interval (time between sync messages)
    simtime_t Emax;         // Maximum error
    //simtime_t nextSync;   // Next synchronization interval
    double startupDelay;
    int numSync;
    bool syncPhase;
    int info_numPkt;
    int numNodes;
    int SN;                     // Packet sequence number
    simtime_t currentOffset;    // Current offset, calculated from the last sync message
    simtime_t lastOffset;
    simtime_t offsetAVG;        // Offset average
    simtime_t maxOffset;
    simtime_t sleepTime;
    simtime_t minOffset;
    double maxVariation;
    vector<offset_info> offset_table;
    double relativeDrift;
    double weight;
    double weightMovingAVG;
    double lastMovingAVG;

protected:
    void startup();
    void fromNetworkLayer(ApplicationPacket *, const char *, double, double);
    void handleRadioControlMessage(RadioControlMessage *);
    void timerFiredCallback(int);
    void finishSpecific();
    simtime_t calcTsync(simtime_t newEmax, double nodeMaxDrift,
            double bsMaxDrift);
    simtime_t nextSync();
    simtime_t maxSleepTime(simtime_t currentNextSync);
};

#endif
