#ifndef MONITOR_H
#define MONITOR_H

#include "ServiceList.h"
#include "WaitList.h"

struct ReturnCheckFeeAndTemp{//返回费用和温度
	float Fee;
	float PreTemp;
};

class Monitor
{
private:
	ServiceList *mServiceList;

	WaitList *mWaitList;

public:
	Monitor();
	~Monitor();

	void setServiceListRelation(ServiceList *list);

	void setWaitListRelation(WaitList *list);

	AirConditionClient* SetupMonitor(int RoomId);//管理员监视用，传入房间号，返回分控机指针

	ReturnCheckFeeAndTemp CheckRoomFee(int RoomId);//住户监视用，传入房间号，返回费用和当前温度
};

#endif // MONITOR_H
