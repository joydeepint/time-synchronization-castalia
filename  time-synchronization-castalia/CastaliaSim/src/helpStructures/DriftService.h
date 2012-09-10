#ifndef CASTALIA_DRIFT
#define CASTALIA_DRIFT

#include <omnetpp.h>
#include <vector>

#include "CastaliaMessages.h"
#include "TimerServiceMessage_m.h"
#include "ResourceManager.h"

struct drift_info {
    int sn;
    simtime_t time;
    double drift;
};

enum DriftVariation {
    DRIFT_NORMAL_ = 5,      // Drift calculated by a normal distribution
    DRIFT_DRASTIC_ = 6,     // Drift with drastic variations
    DRIFT_STATIC_ = 7       // Drift with no variation (Castalia base implementation)
};

class DriftService: public virtual cSimpleModule
{
private:
    ResourceManager *resMgr;        // a pointer to the Resource Manager module
    double clockDrift;              // current clock drift
    vector<drift_info> drift_table; // to save the drift variation during simulation (for output)
    bool driftUp;
    bool driftDown;
    int SN;                     // Sequence number
    double maxDrift;            // Maximum drift rate of the node
    double minDrift;            // Minimum drift rate of the node
    double maxVariation;        // Maximum variation from the current drift
    int driftType;              // 5-normal, 6-drastic and 7-constant
protected:
    void setDriftResourceModule(ResourceManager *resMgr);
    void updateDriftTable(int sn, double newDrift, simtime_t newTime);
    void updateDrift(int driftType, simtime_t time);
    vector<drift_info> getDriftTable() const;
    int getDriftType() const;
    std::vector<TimerServiceMessage*> getTimerMessages() const;
    void setDriftDown(bool driftDown);
    void setDriftType(int driftType);
    void setDriftUp(bool driftUp);
    void setMaxDrift(double maxDrift);
    void setMaxVariation(double maxVariation);
    void setMinDrift(double minDrift);
    void setSn(int sn);
    double getMaxDrift() const;
};

#endif
