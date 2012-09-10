
package node.application.newSyncProtocol;


simple NewSyncProtocol like node.application.iApplication {
 parameters:
 	string applicationID = default ("newSyncProtocol");
	bool collectTraceInfo = default (true);
	int priority = default (1);
	int packetHeaderOverhead = default (5);		// in bytes
	int constantDataPayload = default (100);	// in bytes
	string nextRecipient = default ("0");
	
	bool canSleep = default (true);
	bool isBS = default (false);
	double Emax = default (0.1);			// seconds
	int numNodes = default (5);
	double maxDrift = default (0.000100);	// 100 microseconds por second
	double minDrift = default (0.000010);
	double maxVariation = default (0.000001);
    int driftType = default (5);
	
	double startupDelay = default (0);	// delay in seconds before the app stars producing packets
	double packet_rate = default (0);	// packets per second, by default we transmit no packets
		
	double latencyHistogramMax = default (200);
	int latencyHistogramBuckets = default (10);

 gates:
 	output toCommunicationModule;
	output toSensorDeviceManager;
	input fromCommunicationModule;
	input fromSensorDeviceManager;
	input fromResourceManager;
	inoutfromCommunicationModule;
}