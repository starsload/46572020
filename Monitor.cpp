#include "monitor.h"
#include<iostream>
using namespace std;
Monitor::Monitor()
{
}
Monitor::~Monitor()
{

}
void Monitor::SetupMonitor(vector<int> RoomIdList,int workstate[],float targetTemp[],float preTemp[],float feeRate[],float fee[],float totalFee[],int fanSpeed[],float duration[])
{
    int w=(int)RoomIdList.size();
    for(int i=0;i<w;i++)
    {
        AirConditionClient* mclient;
        if ( mclient = mWaitList->FindACC(RoomID)) {
            workstate[i]=mclient->GetState();
            targetTemp[i]=mclient->GetTargetTemp();
            preTemp[i]=mclient->GetPreTemp();
            feeRate[i]=mclient->GetFeeRate();
            fee[i]=mclient->GetFee();
            totalFee[i]=mclient->GetToalFee();
            fanSpeed[i]=mclient->GetFanSpeed();
            duration[i]=mclient->GetDuration();
        }
        else if (mclient = waitList->FindACC(RoomID)) {
            workstate[i]=mclient->GetState();
            targetTemp[i]=mclient->GetTargetTemp();
            preTemp[i]=mclient->GetPreTemp();
            feeRate[i]=mclient->GetFeeRate();
            fee[i]=mclient->GetFee();
            totalFee[i]=mclient->GetToalFee();
            fanSpeed[i]=mclient->GetFanSpeed();
            duration[i]=mclient->GetDuration();
        }
    }
}
