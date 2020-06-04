#include "servicelist.h"
#include<vector>
#include<iostream>
#include"AirConditionClient.h"
using namespace std;
ServiceList::ServiceList()
{

}
ServiceList::~ServiceList()
{

}

int ServiceList::GetMinPriority()
{
        vector<AirConditionClient>::iterator it = mClientList.begin();
        int min = 9999;
        for (; it != mClientList.end(); it++)
        {
                AirConditionClient air = *it;
                if (air.GetPriority() < min)
                {
                        min = air.GetPriority();
                }
        }
        return min;
}

AirConditionClient* ServiceList::GetAndPopVictim(int RoomId)
{
        vector<AirConditionClient>::iterator it = mClientList.begin();
        int min = mClientList[0].priority;
        int flag;
        int time = 0;
        for (; it != mClientList.end(); it++)
        {
                AirConditionClient air = *it;
                if (air.GetPriority() < min)
                {
                        min = air.GetPriority();
                        if (air.GetServiceTime > time)
                        {
                                time = air.GetServiceTime;
                        }
                }
        }
        for (; it != mClientList.end(); it++)
        {
            AirConditionClient air = *it;
                if (air.GetPriority() == min)
                {
                        if (air.GetServiceTime == time)
                        {
                                flag = air.GetRoomId();
                                it->~AirConditionClient();
                                it = mClientList.erase(it);
                                AirConditionClient* re = new  AirConditionClient(air);
                                return re;
                        }
                }
        }
}
bool ServiceList::isEmpty()
{
        return mClientList.empty();
}

AirConditionClient* ServiceList::PopACC(int RoomId)
{
        vector<AirConditionClient>::iterator it = mClientList.begin();
        for (; it != mClientList.end(); )
        {
            AirConditionClient air = *it;
                if (air.GetRoomId() == RoomId)
                {
                    it = mClientList.erase(it);
                    AirConditionClient *re = new  AirConditionClient(air);
                    return re;
                }
                else it++;
        }

}


void ServiceList::PushACC(AirConditionClient* airConditionClient)
{
    mClientList.push_back(*airConditionClient);
}
void ServiceList::StartRunning(int RoomId)
{
        vector<AirConditionClient>::iterator it = mClientList.begin();
        for (; it != mClientList.end(); )
        {
                AirConditionClient air = *it;
                if (air.GetRoomId() == RoomId)
                {

                        *it = air;
                        break;
                }
                else it++;
        }
}
AirConditionClient* ServiceList::FindACC(int RoomId)
{
        vector<AirConditionClient>::iterator it = mClientList.begin();
        for (; it != mClientList.end(); )
        {
                AirConditionClient air = *it;
                if (air.GetRoomId() == RoomId)
                {
                        return &(*it);
                }
                else it++;
        }
        return NULL;
}


