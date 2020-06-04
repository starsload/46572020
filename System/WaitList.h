#ifndef WAITLIST_H
#define WAITLIST_H
#include<iostream>
#include<vector>
#include"AirConditionClient.h"
using namespace std;

class WaitList
{
private:
        vector<AirConditionClient> mClientList;
public:
        WaitList();
        ~WaitList();
        void Initial();
        bool isEmpty();
        int GetMaxPriority();
        int ReadyNum();
        AirConditionClient* GetAndPopFrontACC();//返回的是拷贝对象
        AirConditionClient* PopACC(int RoomId);//返回的是拷贝对象
        void PushACC(AirConditionClient* airConditionClient);
        AirConditionClient* FindACC(int RoomId);
};


#endif // WAITLIST_H
