#include "WaitList.h"
#include <vector>
#include <iostream>
#include "AirConditionClient.h"
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
        vector<AirConditionClient*>::iterator it = mClientList.begin();
        int max = 0;
        int flag;
        for (; it != mClientList.end(); it++)
        {
                if ((*it)->GetPriority() > max)
                {
                        max = (*it)->GetPriority();
                }
        }
        return max;
}

int WaitList::ReadyNum()
{
	int num = 0;
	for(auto client : mClientList){
		if(client->GetState() == AirConditionClient::STATE_WAIT)
			num++;
	}
	return num;
}

bool WaitList::isEmpty()
{
        return mClientList.empty();
}

AirConditionClient* WaitList::GetAndPopFrontACC()//假定优先级越高数字越大
{
        vector<AirConditionClient*>::iterator it = mClientList.begin();
        int max = GetMaxPriority();
        int flag;
        int time = 0;
		for (; it != mClientList.end(); ) {
			if ((*it)->GetPriority() == max) {
				AirConditionClient *re =*it;
				mClientList.erase(it);

				return re;
			}
			else it++;
        }
}

AirConditionClient* WaitList::PopACC(int RoomId)
{
        vector<AirConditionClient*>::iterator it = mClientList.begin();
        for (; it != mClientList.end(); )
        {
             if ((*it)->GetRoomId() == RoomId)
                {
                        AirConditionClient *re =*it;
                        it = mClientList.erase(it);
                        return re;

                }
                else it++;
        }
}

void WaitList::PushACC(AirConditionClient* airConditionClient)
{
    mClientList.push_back(airConditionClient);
}

AirConditionClient* WaitList::FindACC(int RoomId)
{
    vector<AirConditionClient*>::iterator it = mClientList.begin();
    for (; it != mClientList.end(); )
    {
       if ((*it)->GetRoomId() == RoomId)
        {
            return *it;
        }
        else it++;
    }
    return NULL;
}

void WaitList::debug(){
    qDebug()<<"WaitList：";
	for(auto q : mClientList){
        qDebug()<<QString("client，%1 room").arg(q->GetRoomId());
	}
}
