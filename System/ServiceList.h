#ifndef SERVICELIST_H
#define SERVICELIST_H
#include<vector>
#include<iostream>
#include"AirConditionClient.h"
using namespace std;

class ServiceList
{
private:
        vector<AirConditionClient> mClientList;
public:
        ServiceList();
        ~ServiceList();
        void Initial();
        bool isEmpty();//队列是否为空
        int GetMinPriority();//得到最小优先级的值
        AirConditionClient* GetAndPopVictim();//找到牺牲者 返回的是该对象
        AirConditionClient* PopACC(int RoomId);//根据ID弹出具体的client 返回的是该对象
        void PushACC(AirConditionClient* airConditionClient);//将一个client对象 push到队列中
        AirConditionClient* FindACC(int RoomId);//根据ID返回具体的client
        bool isFull();//判断是否满
};
#endif // SERVICELIST_H
