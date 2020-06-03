#include "AirConditionClient.h"
#include <QPainter>
#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QApplication>
#include <QPushButton>
#include <QtCore/qmath.h>
#include <QFileDialog>
#include <QFileInfo>
#include <QVector>
#include "UseDatabase.h"

void AirConditionClient::Initialize(int RoomId,QString mode,int TargetTemp,int PreTemp,float FeeRate,int FanSpeed,QSqlDatabase db)//初始化函数,对分控机实例的属性进行初始化
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

int AirConditionClient::GetPriority()//获得分控机优先级
{
    return this->priority;
}

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


