cplusplus {{
#include "ApplicationPacket_m.h"
}}
class ApplicationPacket;

packet syncPacket  extends ApplicationPacket {
    
    simtime_t trsTime;
	simtime_t rcvTime;
	simtime_t Emax;
	double maxDrift;
	simtime_t Tsync;  	
}

