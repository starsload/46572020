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
        bool isEmpty();//队列是否为空
        int GetMaxPriority();//得到最大优先级的值
        int ReadyNum();//求队列所含client数
        AirConditionClient* GetAndPopFrontACC();//找出竞争胜利者并返回
        AirConditionClient* PopACC(int RoomId);//根据ID弹出具体的client 返回的是该对象
        void PushACC(AirConditionClient* airConditionClient); //将一个client对象 push到队列中
        AirConditionClient* FindACC(int RoomId);//根据ID返回具体的client
};


#endif // WAITLIST_H
