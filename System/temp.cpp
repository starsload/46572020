#include "AirConditionClient.h"
#include "airconditionhost.h"
#include "UseDatabase.h"
#include "WaitList.h"
#include "ServiceList.h"

void RearchTargetTemp(AirConditionHost *host, ServiceList *serviceList,WaitList *waitList,int RoomId)//到达目标温度进行的调度,参数为主机、服务队列、等待队列以及需要处理的分控机房间号
{
    //找出分控机对象
    AirConditionClient *client;
    AirConditionClient *temp;

    //移出服务队列
    client = serviceList->PopACC(RoomId);

    //设置分控机对象参数
    client->StopRunning();
    client->SetSleep();//状态为休眠

    //调度部分
    if(!waitList->isEmpty())//等待队列不为空发生调度
    {
        //将等待队列最高优先级分控机取出放入服务队列
        temp = waitList->GetAndPopFrontACC();
        serviceList->PushACC(temp);

        //改变分控机状态并分配时间片
        temp->StartRunning();
        temp->DestributeRunTime();
    }

    //将完成服务的分控机移入等待队列
    waitList->PushACC(client);
    waitList->ReadyNum();
}


void TimeFrameOff(AirConditionHost *host, ServiceList *serviceList,WaitList *waitList,int RoomId)//时间片到的调度
{
    AirConditionClient *client,*temp ;
    int max = waitList->GetMaxPriority();//找到等待队列中最高优先级的分控机的优先级

    if(max<serviceList->FindACC(RoomId)->GetPriority())//时间片到达的分控机优先级更高
    {
        //重新分配时间片
        client = serviceList->FindACC(RoomId);
        client->DestributeRunTime();
    }
    else//等待队列中有优先级更高的分控机存在
    {
       //将服务队列中的分控机取出
       client = serviceList->FindACC(RoomId);
       serviceList->PopACC(RoomId);
       waitList->PushACC(client);
       client->StopRunning();


       //将等待队列中的分控机放入服务队列
       temp = waitList->GetAndPopFrontACC();
       serviceList->PushACC(temp);
       //改变分控机状态并分配时间片
       temp->StartRunning();
       temp->DestributeRunTime();
    }
}
