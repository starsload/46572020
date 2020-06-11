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

AirConditionClient* ServiceList::GetAndPopVictim()//排除优先级最低的
{
//        vector<AirConditionClient*>::iterator it = mClientList.begin();
//        int min = mClientList[0]->GetPriority();
//        int time = 0;
//        for (; it != mClientList.end(); it++)
//        {
//                if ((*it)->GetPriority() < min)
//                {
//                        min = (*it)->GetPriority();
//                        if ((*it)->GetDuration() > time)
//                        {
//                                time =(*it)->GetDuration();
//                        }
//                }
//        }
//        for (; it != mClientList.end(); it++)
//        {
//              if ((*it)->GetPriority() == min)
//                {
//                        if ((*it)->GetDuration() == time)
//                        {
//                                AirConditionClient* re = *it;
//                                mClientList.erase(it);
//                                return re;
//                        }
//                }
//        }

		vector<AirConditionClient*>::iterator it = mClientList.begin();
		AirConditionClient *victim = *it;
		int min = mClientList[0]->GetPriority();
		int time = 0;
		for (; it != mClientList.end(); it++)
		{
				if ((*it)->GetPriority() < min)
				{
						min = (*it)->GetPriority();
						victim = *it;
				}
		}
		return victim;
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
	qDebug()<<"ServiceList的内容：";
	for(auto q : mClientList){
		qDebug()<<QString("分控机，%1号房间").arg(q->GetRoomId());
	}
}
