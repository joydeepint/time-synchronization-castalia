
#ifndef CASTALIA_TIMER
#define CASTALIA_TIMER

#include <omnetpp.h>
#include <vector>

#include "CastaliaMessages.h"
#include "ResourceManager.h"

class TimeSyncService: public virtual cSimpleModule {
 private:
	double timerDrift;


 protected:

	virtual void timerFiredCallback(int index);

};

#endif
