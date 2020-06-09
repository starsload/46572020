#ifndef MONITOR_H
#define MONITOR_H

#include "ServiceList.h"
#include "WaitList.h"

class Monitor
{
private:
	ServiceList mServiceList;
	WaitList mWaitList;
public:
	Monitor();
	~Monitor();
	int *SetupMonitor(vector<AirConditionClient> RoomIdList);
};

#endif // MONITOR_H
