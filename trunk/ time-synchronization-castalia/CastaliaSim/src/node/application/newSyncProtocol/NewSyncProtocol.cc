#include "NewSyncProtocol.h"

Define_Module(NewSyncProtocol);

void NewSyncProtocol::startup() {
    isBS = par("isBS");
    Emax = par("Emax");
    startupDelay = par("startupDelay");
    numNodes = par("numNodes");
    canSleep = par("canSleep");
    setMaxDrift((double) par("maxDrift"));
    setMinDrift((double) par("minDrift"));
    setMaxVariation((double) par("maxVariation"));
    setDriftType((int) par("driftType"));
    setSn(0);
    setDriftUp(true);
    setDriftDown(false);
    numSync = 0;
    offsetAVG = 0.0;
    currentOffset = 0.0;
    lastOffset = 0.0;
    Tsync = 6000;
    sleepTime = 0.0;
    syncPhase = true;
    info_numPkt = 0;
    maxOffset = 0.0;
    minOffset = 1000.0;
    offsetAVG = 0.0;
    relativeDrift = 0.0;
    weight = 0.1;
    weightMovingAVG = 0.0;

    //set the ResourceManager for DriftService
    setDriftResourceModule(resMgrModule);

    setTimer(SEND_INFO, startupDelay);
    setTimer(DRIFT_UPDATE, resMgrModule->getDriftSampleTime() + startupDelay);
    trace() << "Maximum error for node " << self << " : " << Emax;
}

void NewSyncProtocol::timerFiredCallback(int timerIndex) {
    switch (timerIndex) {

    case SEND_SYNC: {
        if (isBS && !syncPhase) {
            syncPacket *pkt = new syncPacket("SYNC", APPLICATION_PACKET);
            toNetworkLayer(pkt, BROADCAST_NETWORK_ADDRESS);
            trace() << "drift: " << getTimerDrift() << "  resourceDrift: "
                    << resMgrModule->getCPUClockDrift();
            setTimer(SEND_SYNC, Tsync);
        } else
            setTimer(SEND_SYNC, Tsync);
        break;
    }
    case RCV_SYNC: {
        if (!isBS) {
            // wake up
            toNetworkLayer(createRadioCommand(SET_STATE, RX));
        }
        break;
    }
    case SEND_INFO: {
        //send initial info to base station
        if (!isBS) {
            syncPacket *infoPkt = new syncPacket("NODE_INFO",
                    APPLICATION_PACKET);
            infoPkt->setEmax(Emax);
            infoPkt->setMaxDrift(getMaxDrift());
            toNetworkLayer(infoPkt, "0");
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

void NewSyncProtocol::fromNetworkLayer(ApplicationPacket * rcvPacket,
        const char *source, double rssi, double lqi) {
    syncPacket *sp = check_and_cast<syncPacket*>(rcvPacket);
    string packetType = rcvPacket->getName();

    if (packetType == "SYNC") {
        trace() << "Packet type:  " << packetType;
        currentOffset = sp->getRcvTime() - sp->getTrsTime();

        trace() << "clock before set: " << getClock();
        setClock(currentOffset);
        trace() << "clock after set: " << getClock();

        trace() << "current drift: " << getTimerDrift() - 1.0f;
        trace() << "Packet Transmitted Time: " << sp->getTrsTime()
                << " Packet Received Time: " << sp->getRcvTime()
                << " current offset (us): " << currentOffset * 1000000;
        trace() << "nextSync: " << nextSync();

        // Calculation of the node clock drift relatively to the base station
        if (sleepTime == 0.0) {
            relativeDrift = currentOffset / sp->getRcvTime();
            weightMovingAVG = weight * relativeDrift
                    + (1 - weight) * relativeDrift;
        } else
            relativeDrift = (currentOffset / sleepTime)
                    + resMgrModule->getDriftCompensation();

        // Calculation of the weight to be used on the weight moving average filter.
        weight = ((relativeDrift - weightMovingAVG) * 1000000) * 0.1;

        if (weight < 0)
            weight = weight * -1;
        if (weight > 0.9)
            weight = 0.9;

        // Calculation of the weight moving average filter.
        weightMovingAVG = weight * relativeDrift
                + (1 - weight) * weightMovingAVG;

        trace() << "Relative Drift: " << relativeDrift;
        trace() << "weight: " << weight;

        trace() << "Drift Compensation: " << weightMovingAVG;
        resMgrModule->setDriftCompensation(weightMovingAVG);

        // set timer for node sleep
        sleepTime = maxSleepTime(nextSync());
        if (canSleep)
        {
            setTimer(RCV_SYNC, sleepTime - 1);
        trace() << "Max Sleep Time: " << sleepTime;
        toNetworkLayer(createRadioCommand(SET_STATE, SLEEP));
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

    if (packetType == "NODE_INFO") {
        trace() << "Packet type:  " << packetType;
        if (isBS) {
            info_numPkt++;
            trace() << "Tsync: " << Tsync;
            simtime_t newTsync = calcTsync(sp->getEmax(), sp->getMaxDrift(),
                    getMaxDrift());
            trace() << "New Tsync: " << newTsync;

            if (newTsync <= Tsync)
                Tsync = newTsync;

            if (info_numPkt >= numNodes) {
                syncPhase = false;
                syncPacket *pkt = new syncPacket("NODE_INFO",
                        APPLICATION_PACKET);
                pkt->setTsync(Tsync);
                toNetworkLayer(pkt, BROADCAST_NETWORK_ADDRESS);
                setTimer(SEND_SYNC, Tsync);
            }
        } else {
            Tsync = sp->getTsync();
            trace() << "Node Tsync: " << Tsync;
            currentOffset = sp->getRcvTime() - sp->getTrsTime();
            setClock(currentOffset);
            numSync++;
        }
    }
}

void NewSyncProtocol::finishSpecific() {
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

        for (int i = 0; i < (int) offset_table.size(); i++) {
            collectOutput("Offset", offset_table[i].sn, "",
                    SIMTIME_DBL(offset_table[i].offset));
        }
        vector<drift_info> driftTable = getDriftTable();
        for (int i = 0; i < (int) driftTable.size(); i++) {
            collectOutput("Drift", driftTable[i].sn, "", driftTable[i].drift);
        }
    }

}

simtime_t NewSyncProtocol::calcTsync(simtime_t newEmax, double nodeMaxDrift,
        double bsMaxDrift) {
    return newEmax / (nodeMaxDrift + bsMaxDrift);
}

simtime_t NewSyncProtocol::nextSync() {
    if (currentOffset < 0)
        return (Emax - (currentOffset * -1.0f)) / getMaxDrift();
    else
        return (Emax - currentOffset) / getMaxDrift();
}

simtime_t NewSyncProtocol::maxSleepTime(simtime_t currentNextSync) {
    if (currentNextSync <= Tsync)
        return Tsync;
    else
        return ((int) (currentNextSync / Tsync)) * Tsync;
}

void NewSyncProtocol::handleRadioControlMessage(RadioControlMessage *radioMsg) {
}

