#include "AirConditionClient.h"

#include <QDebug>
#include <QtCore/qmath.h>
#include <QFileInfo>
#include <QVector>
#include <QSqlDatabase>
#include "UseDatabase.h"
#include <synchapi.h>


//AirConditionClient::AirConditionClient(QObject *parent):
//	QObject(parent)
//{
//	timer = new QTimer();
//	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
//	timer->start(1000);
//}

AirConditionClient::AirConditionClient(){

}

AirConditionClient::AirConditionClient(const AirConditionClient& host){
	this->timer = host.timer;
}

AirConditionClient::~AirConditionClient(){}

void AirConditionClient::DestributeRunTime(){}

void AirConditionClient::Initialize(int RoomId,int mode,int TargetTemp,int PreTemp,float FeeRate,int FanSpeed,QSqlDatabase db)//初始化函数,对分控机实例的属性进行初始化
{
    //向服务器发送初始化请求


    //向数据库查询总花费参数
    QString str = QString("select TOTAL_FEE from ACC_chart where ROOM_ID = %1").arg(RoomId);
    QSqlQuery query(db);
    query.exec(str);
    query.first();

    //初始化参数
    this->RoomId = RoomId;
    this->mode = mode;
    this->PreTemp = PreTemp;
    this->TargetTemp = TargetTemp;
    this->FeeRate = FeeRate;
    this->Fee = 0;
    this->TotalFee = query.value(0).toFloat();
    this->FanSpeed = FanSpeed;
    this->priority = 0;
    this->Duration = 0;
    this->get_server_time = "";
    this->stop_server_time = "";
}

void AirConditionClient::SetSpeed(int FanSpeed)//设置分控机风速
{
    this->FanSpeed = FanSpeed;
}

void AirConditionClient::SetTargetTemp(int TargetTemp)//设置分控机目标温度
{
    this->TargetTemp = TargetTemp;
}

int AirConditionClient::GetPriority(){return this->priority;}//获得分控机优先级
int AirConditionClient::GetRoomId() {return this->RoomId;} 
int AirConditionClient::Getwork_state() {	return this->work_state;} 
int AirConditionClient::Getmode() { return this->mode; }
float AirConditionClient::GetTargetTemp() { return this->TargetTemp; }
float AirConditionClient::GetPreTemp() { return this->PreTemp; }
float AirConditionClient::GetFeeRate() { return this->FeeRate; }
float AirConditionClient::GetFee() { return this->Fee; }
float AirConditionClient::GetTotalFee() { return this->TotalFee; }
int AirConditionClient::GetFanSpeed() { return this->FanSpeed; }
int AirConditionClient::Getpriority() { return this->priority; }
float AirConditionClient::GetDuration() { return this->Duration; }
QString AirConditionClient::Getget_server_time() { return this->get_server_time ;};
QString AirConditionClient::Getstop_server_time() { return this->stop_server_time; };

QVector<float> AirConditionClient::GetFinalState()//获得关机时分控机状态
{
    QVector<float> List;//暂存参数
    List.append(this->Fee);
    List.append(this->Duration);
    return List;
}

int AirConditionClient::GetState()//获得分控机当前状态
{
    return this->work_state;//0休眠，1等待，2运行
}

void AirConditionClient::ReachTargetTemperature()//到达目标温度
{
    //改变自身参数
    this->work_state=0;//进入休眠状态
}

QVector<int> AirConditionClient::TimeOff()//时间片到
{
    //向服务器提供自身房间号和费率
    QVector<int> List;
    List.append(this->RoomId);
    List.append(this->FeeRate);
    return List;
}

bool AirConditionClient::isRunning()//判断分控机对象是否运行
{
    if(this->work_state==2)
        return true;
    else return false;
}

void AirConditionClient::StopRunning()//分控机停止运行
{
    this->work_state=1;
}

void AirConditionClient::StartRunning()//开始运行
{
    this->work_state=2;
}

void AirConditionClient::Delete()//删除分控机
{
}

void AirConditionClient::Recovery_temp()//回温算法,每分钟变化0.5度
{
    if(this->TargetTemp > This->PreTemp)
    {
        float s = this->TargetTemp;
        for(; s >= this->PreTemp; )
        {
            s = s - 0.5;
            this->TargetTemp = s;
            msleep(60000);//休眠1分钟
        }
    }
    if(this->TargetTemp < This->PreTemp)
    {
        int s = this->TargetTemp;
        for(; s >= this->PreTemp; )
        {
            s = s + 0.5;
            this->TargetTemp = s;
            msleep(60000);//休眠1分钟
        }
    }
}
