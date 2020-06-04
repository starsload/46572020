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
        bool isEmpty();
        int GetMinPriority();
        AirConditionClient* GetAndPopVictim(int RoomId);//返回的是拷贝对象
        AirConditionClient* PopACC(int RoomId);//返回的是拷贝对象
        void PushACC(AirConditionClient* airConditionClient);
        void StartRunning(int RoomId);
        AirConditionClient* FindACC(int RoomId);
};
#endif // SERVICELIST_H
