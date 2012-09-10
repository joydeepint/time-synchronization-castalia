
#ifndef _FTSP_H_
#define _FTSP_H_

#include "VirtualApplication.h"
#include "syncPacket_m.h"


using namespace std;

#define MAX_ENTRIES 8   // maximum number of entries for linear regression

struct offset_info {
    int sn;
    simtime_t time;
    simtime_t offset;
};

enum Timers {
    SEND_SYNC = 1,          // Timer to send sync messages (base station)
    DRIFT_UPDATE = 39       // Timer to update drift
};

typedef struct TableItem {
    double localTime;
    double globalTime;
    double global_local;
    double global_global;

} TableItem;

class FTSP: public VirtualApplication{
private:


    bool isBS; // To identify the base station
    double Tsync;
    double startupDelay;
    int numNodes;
    int numSync;
    simtime_t offsetAVG;
    simtime_t minOffset;
    vector<offset_info> offset_table;
    simtime_t currentOffset;            // Current offset, calculated from the last sync message
    simtime_t lastOffset;
    simtime_t maxOffset;

    TableItem tableLR[MAX_ENTRIES];
    double slope;
    double intercept;
    double sumLocalTime;
    double sumGlobalTime;
    double sumGlobal_Local;
    double sumGlobal_Global;
    int numEntries;

protected:
    void startup();
    void fromNetworkLayer(ApplicationPacket *, const char *, double, double);
    void handleRadioControlMessage(RadioControlMessage *);
    void timerFiredCallback(int);
    void finishSpecific();
};

#endif
