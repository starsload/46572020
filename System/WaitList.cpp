#include "waitlist.h"
#include<vector>
#include<iostream>
#include"AirConditionClient.h"
using namespace std;
WaitList::WaitList()
{

}

WaitList::~WaitList()
{

}

void WaitList::Initial(){}

int WaitList::GetMaxPriority()
{
        vector<AirConditionClient>::iterator it = mClientList.begin();
        int max = 0;
        int flag;
        for (; it != mClientList.end(); it++)
        {
				AirConditionClient &air = *it;
                if (air.GetPriority() > max)
                {
                        max = air.GetPriority();
                }
        }
        return max;
}

int WaitList::ReadyNum()
{
        return mClientList.size();
}
bool WaitList::isEmpty()
{
        return mClientList.empty();
}

AirConditionClient* WaitList::GetAndPopFrontACC()//假定优先级越高数字越大
{
        vector<AirConditionClient>::iterator it = mClientList.begin();
        int max = GetMaxPriority();
        int flag;
        int time = 0;
//        for (; it != mClientList.end(); )
//        {
//                AirConditionClient air = *it;
//                if (air.GetPriority() == max)
//                {
//                        if (air.WaitServiceTime > time)
//                        {
//                                time = air.WaitServiceTime;
//                        }
//                }
//                else it++;
//        }
        for (; it != mClientList.end(); )
        {
            AirConditionClient air = *it;
                if (air.GetPriority() == max)
                {
                        //if (air.WaitServiceTime == time)
                       // {
                                flag = air.GetRoomId();
                                it->~AirConditionClient();
                                it = mClientList.erase(it);
                                AirConditionClient *re = new  AirConditionClient(air);
                                return re;
                      //  }
                }
                else it++;
        }
}
AirConditionClient* WaitList::PopACC(int RoomId)
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

void WaitList::PushACC(AirConditionClient* airConditionClient)
{
    mClientList.push_back(*airConditionClient);
}

AirConditionClient* WaitList::FindACC(int RoomId)
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
