#include "monitor.h"

Monitor::Monitor()
{
}

Monitor::~Monitor()
{

}

void Monitor::setServiceListRelation(ServiceList *list){
	this->mServiceList = list;
}

void Monitor::setWaitListRelation(WaitList *list){
	this->mWaitList = list;
}

AirConditionClient* Monitor::SetupMonitor(int RoomId)//管理员监视用，传入房间号，返回分控机指针
{
	AirConditionClient* client = NULL;
	client = mWaitList->FindACC(RoomId);
	if(client != NULL){//在等待队列
		return client;
	}
	else{//在服务队列
		client = mServiceList->FindACC(RoomId);
		if(client != NULL)
			return client;
		else
			return nullptr;
	}
}

ReturnCheckFeeAndTemp Monitor::CheckRoomFee(int RoomId) //住户监视用，传入房间号，返回费用和当前温度
{
	AirConditionClient* client = NULL;
	struct ReturnCheckFeeAndTemp temp;
	client = mWaitList->FindACC(RoomId);
	if(client != NULL){//在等待队列
		temp.Fee = client->GetFee();
		temp.PreTemp = client->GetPreTemp();
		temp.TotalFee = client->GetTotalFee();
	}
	else{//在服务队列
		client = mServiceList->FindACC(RoomId);
		temp.Fee = client->GetFee();
		temp.PreTemp = client->GetPreTemp();
		temp.TotalFee = client->GetTotalFee();
	}
	return temp;
}
