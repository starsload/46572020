#include "AirConditionClient.h"

#include <QDebug>
#include <QtCore/qmath.h>
#include <QFileInfo>
#include <QVector>
#include <QSqlDatabase>
#include "UseDatabase.h"
#include <QTimer>
#include <QTime>
#include <QDateTime>

AirConditionClient::AirConditionClient(){
	TimeSliceTimer = new QTimer();
	updateTimer = new QTimer();
	QObject::connect(this->TimeSliceTimer,SIGNAL(timeout()),this,SLOT(WhenTimeOff()));
	connect(updateTimer, SIGNAL(timeout), this, SLOT(updateAttribute()));
}

AirConditionClient::~AirConditionClient(){
	delete TimeSliceTimer;
	delete updateTimer;
}

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
    this->get_server_time = QDateTime::currentDateTime();
    this->stop_server_time = QDateTime::currentDateTime();
}

void AirConditionClient::SetSpeed(int FanSpeed)//设置分控机风速
{
    this->FanSpeed = FanSpeed;
    this->stop_server_time = QDateTime::currentDateTime();//获取结束时间
    qint64 temp = this->get_server_time.secsTo(this->stop_server_time);
    this->Duration = this->Duration + float(temp)/60;
    this->Fee=this->Fee+this->Duration*(this->FeeRate-1/FanSpeed);//这样可以保证计算Fee时算的是变更后的结果
    this->FeeRate = 1/FanSpeed;//计费相关
}

void AirConditionClient::SetTargetTemp(int TargetTemp)//设置分控机目标温度
{
    this->TargetTemp = TargetTemp;
}

int AirConditionClient::GetPriority(){
	return this->priority;
}//获得分控机优先级

int AirConditionClient::GetRoomId() {
	return this->RoomId;
}

int AirConditionClient::Getwork_state() {
	return this->work_state;
}

int AirConditionClient::Getmode() {
	return this->mode;
}

int AirConditionClient::GetTargetTemp() {
	return this->TargetTemp;
}

int AirConditionClient::GetPreTemp() {
	return this->PreTemp;
}

float AirConditionClient::GetFeeRate() {
	return this->FeeRate;
}

float AirConditionClient::GetFee() {
	return this->Fee+this->Duration*this->FeeRate;
}

float AirConditionClient::GetTotalFee() {
	return this->TotalFee+this->Fee+this->Duration*this->FeeRate;
}

int AirConditionClient::GetFanSpeed() { return this->FanSpeed; }

int AirConditionClient::Getpriority() { return this->priority; }

int AirConditionClient::GetDuration() { return this->Duration; }

QString AirConditionClient::Getget_server_time() {
	return this->get_server_time.toString("yyyy-MM-dd");
};

QString AirConditionClient::Getstop_server_time() {
	return this->stop_server_time.toString("yyyy-MM-dd");
};

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

QVector<int> AirConditionClient::TimeOff()//时间片到，发送费率和房间号
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
	this->TimeSliceTimer->stop();
	updateTimer->stop();
    this->stop_server_time = QDateTime::currentDateTime();//获取结束时间

    //计算时间差
    qint64 temp = this->get_server_time.secsTo(this->stop_server_time);
    //添加到运行时间，以分钟计算
    this->Duration = this->Duration + float(temp)/60;
}

void AirConditionClient::StartRunning()//开始运行
{
    this->work_state=2;
    this->get_server_time = QDateTime::currentDateTime();//获取当前时间
}

void AirConditionClient::SetSleep()//设置状态为休眠
{
    this->work_state = 0;
}

void AirConditionClient::DestributeRunTime()//分配运行时间片
{

	this->TimeSliceTimer->start(120000);//两分钟触发一次
	updateTimer->start(10 * SECOND);
}

void AirConditionClient::WhenTimeOff()//当时间片到
{
    //报告费率
    QVector<int> temp;
    temp = this->TimeOff();//有两位，第一位存房间号，第二位存费率

	this->TimeSliceTimer->stop();//停止计时器
	updateTimer->stop();
}

//更新温度和费用
void AirConditionClient::updateAttribute(){
	//高风速1分钟变化1℃，中风速2分钟变化1℃，低风速3分钟变化1℃
	float deltaTemp = 0;
	switch(FanSpeed){
	case 0:
	{
		deltaTemp = UPDATE_PERIOD / (3 * MINUTE);
		break;
	}
	case 1:
	{
		deltaTemp = UPDATE_PERIOD / (2 * MINUTE);
		break;
	}
	case 2:
	{
		deltaTemp = UPDATE_PERIOD / (1 * MINUTE);
		break;
	}
	}
	if(mode)//制热
		PreTemp += deltaTemp;
	else//制冷
		PreTemp -= deltaTemp;
	//1元/摄氏度
	Fee += deltaTemp * 1;
}
