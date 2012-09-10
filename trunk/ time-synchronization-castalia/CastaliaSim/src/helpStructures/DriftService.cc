#include "DriftService.h"

// Pointer to the Resource Manager
void DriftService::setDriftResourceModule(ResourceManager *resMgr) {
    this->resMgr = resMgr;
}
vector<drift_info> DriftService::getDriftTable() const
{
    return drift_table;
}

int DriftService::getDriftType() const
{
    return driftType;
}

void DriftService::setDriftDown(bool driftDown)
{
    this->driftDown = driftDown;
}

void DriftService::setDriftType(int driftType)
{
    this->driftType = driftType;
}

void DriftService::setDriftUp(bool driftUp)
{
    this->driftUp = driftUp;
}

void DriftService::setMaxDrift(double maxDrift)
{
    this->maxDrift = maxDrift;
}

void DriftService::setMaxVariation(double maxVariation)
{
    this->maxVariation = maxVariation;
}

void DriftService::setMinDrift(double minDrift)
{
    this->minDrift = minDrift;
}

double DriftService::getMaxDrift() const
{
    return maxDrift;
}

void DriftService::setSn(int sn)
{
    SN = sn;
}
//Track the drift variation during simulation time
void DriftService::updateDriftTable(int sn, double newDrift, simtime_t newTime) {

    int pos = -1;
        for (int i = 0; i < (int) drift_table.size(); i++) {
            if (drift_table[i].sn == SN)
                pos = i;
        }

        drift_info new_info_Drift;
        new_info_Drift.sn = sn;
        new_info_Drift.drift = newDrift;
        new_info_Drift.time = newTime;
        drift_table.push_back(new_info_Drift);

}
// Update the drift value based on the drift type. The new value is returned by the ResourceManager.
void DriftService::updateDrift(int index, simtime_t time) {
    switch (index) {
         case DRIFT_DRASTIC_: {
                if (driftUp == true) {
                    if (resMgr->getCPUClockDrift() < maxDrift + 1.0f) {
                        clockDrift = uniform(
                                (resMgr->getCPUClockDrift() - 1.0f),
                                (resMgr->getCPUClockDrift() - 1.0f)
                                        + maxVariation);

                        resMgr->updateDrift(clockDrift, time);
                    } else {
                        driftUp = false;
                        driftDown = true;
                    }
                }
                if (driftDown == true) {
                    if (resMgr->getCPUClockDrift() > minDrift + 1.0f) {
                        clockDrift = uniform(
                                (resMgr->getCPUClockDrift() - 1.0f)
                                        - maxVariation,
                                (resMgr->getCPUClockDrift() - 1.0f));

                        resMgr->updateDrift(clockDrift, time);
                    } else {
                        clockDrift = uniform(
                                (resMgr->getCPUClockDrift() - 1.0f),
                                (resMgr->getCPUClockDrift() - 1.0f)
                                        + maxVariation);

                        resMgr->updateDrift(clockDrift, time);
                        driftUp = true;
                        driftDown = false;
                    }
                }
                updateDriftTable(SN,
                        (resMgr->getCPUClockDrift() - 1.0f) * 1000000,
                        simTime());
                SN++;
            break;
        }

        case DRIFT_NORMAL_: {
                //cpuClockDrift = normal(resMgrModule->getCPUClockDrift()-1.0f, maxVariation);
                clockDrift = uniform(
                        (resMgr->getCPUClockDrift() - 1.0f)
                                - maxVariation,
                        (resMgr->getCPUClockDrift() - 1.0f)
                                + maxVariation);

                if (clockDrift <= maxDrift && clockDrift >= minDrift)
                    resMgr->updateDrift(clockDrift, time);
                else
                    resMgr->updateDrift(
                            (resMgr->getCPUClockDrift() - 1.0f),
                            time);

                updateDriftTable(SN,
                        (resMgr->getCPUClockDrift() - 1.0f) * 1000000,
                        simTime());
                SN++;
            break;
        }

        case DRIFT_STATIC_: {
                clockDrift = resMgr->getCPUClockDrift() - 1.0f;
                resMgr->updateDrift(clockDrift, time);

                updateDriftTable(SN,
                        (resMgr->getCPUClockDrift() - 1.0f) * 1000000,
                        simTime());
                SN++;
          break;
        }
    }
}


