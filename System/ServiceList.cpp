#include "ServiceList.h"
#include <vector>
#include <iostream>
#include "AirConditionClient.h"
using namespace std;

ServiceList::ServiceList()
{

}
ServiceList::~ServiceList()
{

}

void ServiceList::Initial(){}

int ServiceList::GetMinPriority()
{
		vector<AirConditionClient*>::iterator it = mClientList.begin();
        int min = 9999;
        for (; it != mClientList.end(); it++)
        {

                if ((*it)->GetPriority() < min)
                {
                        min = (*it)->GetPriority();
                }
        }
        return min;
}


//修改了下面的整个函数
AirConditionClient* ServiceList::GetAndPopVictim()//排除优先级最低的
{
	vector<AirConditionClient*>::iterator it = mClientList.begin();

        AirConditionClient *victim = NULL;
        int min = GetMinPriority();
		int time = 0;
		for (; it != mClientList.end(); it++)
		{
                if ((*it)->GetPriority() == min)
			{

						victim = *it;
                        mClientList.erase(it);
                        return victim;

			}
		}
}

bool ServiceList::isEmpty()
{
	if(mClientList.size() == 0)
		return true;
	else
		return false;
}

AirConditionClient* ServiceList::PopACC(int RoomId)
{
        vector<AirConditionClient*>::iterator it = mClientList.begin();
        for (; it != mClientList.end(); )
        {

                if ((*it)->GetRoomId() == RoomId)
                {
                    AirConditionClient* re = *it;
                    mClientList.erase(it);
                    return re;
                }
                else it++;
        }

}


void ServiceList::PushACC(AirConditionClient* airConditionClient)
{
    mClientList.push_back(airConditionClient);
}

AirConditionClient* ServiceList::FindACC(int RoomId)
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
		return nullptr;
}

bool ServiceList::isFull()
{
    if(this->mClientList.size() == 3)
        return true;
    else return false;
}

int ServiceList::getSize(){
	return mClientList.size();
}

void ServiceList::debug(){
    qDebug()<<"ServiceList：";
	for(auto q : mClientList){
        qDebug()<<QString("client，%1 room").arg(q->GetRoomId());
	}
}
