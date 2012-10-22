#include "FTSP.h"

Define_Module(FTSP);

void FTSP::startup() {
    isBS = par("isBS");
    Tsync = par("Tsync");
    startupDelay = par("startupDelay");
    numNodes = par("numNodes");
    setMaxDrift((double) par("maxDrift"));
    setMinDrift((double) par("minDrift"));
    setMaxVariation((double) par("maxVariation"));
    setDriftType((int) par("driftType"));
    setSn(0);
    setDriftUp(true);
    setDriftDown(false);
    numEntries = 0;
    numSync = 0;
    currentOffset = 0.0;
    lastOffset = 0.0;
    maxOffset = 0.0;
    minOffset = 1000.0;
    offsetAVG = 0.0;

    //set the ResourceManager in DriftService
    setDriftResourceModule(resMgrModule);
    setTimer(DRIFT_UPDATE, resMgrModule->getDriftSampleTime() + startupDelay);
    setTimer(SEND_SYNC, Tsync + startupDelay);

}

void FTSP::timerFiredCallback(int timerIndex) {
    switch (timerIndex) {
    case SEND_SYNC: {
        if (isBS) {
            syncPacket *pkt = new syncPacket("SYNC", APPLICATION_PACKET);

            toNetworkLayer(pkt, BROADCAST_NETWORK_ADDRESS);
            trace() << "drift: " << getTimerDrift() << "  resourceDrift: "
                    << resMgrModule->getCPUClockDrift();
            setTimer(SEND_SYNC, Tsync);
        }
        break;
    }
    case DRIFT_UPDATE: {
        if (!isBS) {
            updateDrift(getDriftType(), getClock());
            setTimer(DRIFT_UPDATE, resMgrModule->getDriftSampleTime());
        }
        break;
    }
    }
}

void FTSP::fromNetworkLayer(ApplicationPacket * rcvPacket, const char *source,
        double rssi, double lqi) {
    syncPacket *sp = check_and_cast<syncPacket*>(rcvPacket);
    simtime_t lrOffset;

    currentOffset = sp->getRcvTime() - sp->getTrsTime();
    lrOffset = (sp->getRcvTime() + resMgrModule->getOffset()
            + resMgrModule->getDriftCompensationAdjust()) - sp->getTrsTime();

    trace() << "clock before set: " << getClock();
    setClock(currentOffset);
    trace() << "clock after set: " << getClock();

    trace() << "current drift: " << getTimerDrift() - 1.0f;
    trace() << "Packet Transmitted Time: " << sp->getTrsTime()
            << " Packet Received Time: " << sp->getRcvTime()
            << " current offset (us): " << currentOffset * 1000000;

    TableItem newItem;
    newItem.globalTime = SIMTIME_DBL(sp->getTrsTime());
    newItem.localTime = SIMTIME_DBL(lrOffset);
    newItem.global_local = SIMTIME_DBL(sp->getTrsTime()) * SIMTIME_DBL(lrOffset);
    newItem.global_global = SIMTIME_DBL(sp->getTrsTime())
            * SIMTIME_DBL(sp->getTrsTime());

    if (numEntries == 0) {
        tableLR[0] = newItem;
        numEntries++;
    } else {
        if (numEntries == MAX_ENTRIES) {
            for (int i = 0; i < MAX_ENTRIES; i++) {
                tableLR[i] = tableLR[i + 1];
            }
            tableLR[numEntries - 1] = newItem;
        } else {
            tableLR[numEntries] = newItem;
            numEntries++;
        }
    }

    // Calculation of the Linear Regression
    sumGlobalTime = 0;
    sumLocalTime = 0;
    sumGlobal_Local = 0;
    sumGlobal_Global = 0;

    if (numEntries >= 2) {
        for (int i = 0; i < numEntries - 1; i++)
            sumGlobalTime += tableLR[i].globalTime;

        for (int i = 0; i < numEntries - 1; i++)
            sumLocalTime += tableLR[i].localTime;

        for (int i = 0; i < numEntries - 1; i++)
            sumGlobal_Local += tableLR[i].global_local;

        for (int i = 0; i < numEntries - 1; i++)
            sumGlobal_Global += tableLR[i].global_global;

        slope =
                ((numEntries * sumGlobal_Local) - (sumGlobalTime * sumLocalTime))
                        / ((numEntries * sumGlobal_Global)
                                - (sumGlobalTime * sumGlobalTime));
        intercept = (sumLocalTime - slope * sumGlobalTime) / numEntries;

        double skew = (lrOffset - intercept) / sp->getTrsTime();

        trace() << "slope: " << slope;
        trace() << "intercept: " << intercept;
        trace() << "skew: " << skew;
        resMgrModule->setDriftCompensation(slope);

    }

    if (numSync > 2) {
        offset_info new_info_offset;
        new_info_offset.sn = numSync - 2;
        new_info_offset.offset = currentOffset * 1000000;
        new_info_offset.time = simTime();
        offset_table.push_back(new_info_offset);

        //maximum and minimum offset of the node (absolute values)
        if (currentOffset < 0)
            currentOffset = currentOffset * (-1.0f);
        if (currentOffset > maxOffset)
            maxOffset = currentOffset;
        if (currentOffset < minOffset)
            minOffset = currentOffset;
        offsetAVG = offsetAVG + currentOffset;
    }

    numSync++;
}

void FTSP::finishSpecific() {

    declareOutput("Worst case error");
    declareOutput("Best case error");
    declareOutput("Average error");
    declareOutput("Number of sync messages");
    declareOutput("Drift");
    declareOutput("Offset");

    if (!isBS) {
        collectOutput("Worst case error", "", SIMTIME_DBL(maxOffset * 1000000));
        collectOutput("Best case error", "", SIMTIME_DBL(minOffset * 1000000));
        collectOutput("Average error", "",
                SIMTIME_DBL((offsetAVG / (numSync -2))*1000000));
        collectOutput("Number of sync messages", "", numSync);
    }
    for (int i = 0; i < (int) offset_table.size(); i++) {
        collectOutput("Offset", offset_table[i].sn, "",
                SIMTIME_DBL(offset_table[i].offset));
    }
    vector<drift_info> driftTable = getDriftTable();
    for (int i = 0; i < (int) driftTable.size(); i++) {
        collectOutput("Drift", driftTable[i].sn, "", driftTable[i].drift);
    }
}

void FTSP::handleRadioControlMessage(RadioControlMessage *radioMsg) {
}

