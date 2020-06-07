#ifndef MONITOR_H
#define MONITOR_H


class Monitor
{
private:
    ServiceList *mServiceList;
    WaitList *mWaitList;
public:
    Monitor();
    ~Monitor();
    void SetupMonitor(vector<AirConditionClient> RoomIdList,int workstate[],float targetTemp[],float preTemp[],float feeRate[],float fee[],float totalFee[],int fanSpeed[],float duration[]);
};

#endif // MONITOR_H
