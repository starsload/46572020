#include "AirConditionClient.h"
#include "AirConditionHost.h"

AirConditionClient::AirConditionClient(){
	TimeSliceTimer = new QTimer();
	updateTimer = new QTimer();
	QObject::connect(this->TimeSliceTimer,SIGNAL(timeout()),this,SLOT(WhenTimeOff()));
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateAttribute()));
}

AirConditionClient::~AirConditionClient(){
	delete TimeSliceTimer;
	delete updateTimer;
}

void AirConditionClient::Initialize(float totalFee,int RoomId,int mode,int TargetTemp,int PreTemp,float FeeRate,int FanSpeed,QSqlDatabase db)//初始化函数,对分控机实例的属性进行初始化
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
    this->TotalFee = totalFee;
    this->FanSpeed = FanSpeed;
	this->priority = FanSpeed + 1;
    this->Duration = 0;

    //修改
    this->work_state = STATE_WAIT;

    this->get_server_time = QDateTime::currentDateTime();
    this->stop_server_time = QDateTime::currentDateTime();
	this->work_state = STATE_WAIT;
}

void AirConditionClient::setAirConditionHostRelation(AirConditionHost *host){
	this->airConditionHost = host;
}

void AirConditionClient::SetSpeed(int FanSpeed)//设置分控机风速
{
    this->FanSpeed = FanSpeed;
    this->stop_server_time = QDateTime::currentDateTime();//获取结束时间
    //计算时间差
    uint stime = this->get_server_time.toTime_t();
    uint etime = this->stop_server_time.toTime_t();

    //添加到运行时间，以分钟计算
    this->Duration = float(etime-stime);
	this->priority = FanSpeed + 1;
}

void AirConditionClient::SetTargetTemp(int TargetTemp)//设置分控机目标温度
{
    this->TargetTemp = TargetTemp;
}

void AirConditionClient::SetPreTemp(float temp){
	this->PreTemp = temp;
}

int AirConditionClient::GetPriority(){
	return this->priority;
}//获得分控机优先级

int AirConditionClient::GetRoomId() {
	return this->RoomId;
}

int AirConditionClient::Getmode() {
	return this->mode;
}

int AirConditionClient::GetTargetTemp() {
	return this->TargetTemp;
}

float AirConditionClient::GetPreTemp() {
	float t = PreTemp;
	return t;
}

float AirConditionClient::GetFeeRate() {
	return this->FeeRate;
}

float AirConditionClient::GetFee() {
	return this->Fee;
}

float AirConditionClient::GetTotalFee() {
	return this->TotalFee;
}

int AirConditionClient::GetFanSpeed() { return this->FanSpeed; }

int AirConditionClient::Getpriority() { return this->priority; }

float AirConditionClient::GetDuration() { return this->Duration; }

QString AirConditionClient::Getget_server_time() {
    return this->get_server_time.toString("yyyy-MM-dd-hh:mm:ss");
};

QString AirConditionClient::Getstop_server_time() {
    return this->stop_server_time.toString("yyyy-MM-dd-hh:mm:ss");
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
	qDebug()<<QString("%1号房间，达到目标温度").arg(RoomId);

	//改变自身参数
	this->work_state = 0;//进入休眠状态
	this->TimeSliceTimer->stop();
	this->updateTimer->stop();

	this->stop_server_time = QDateTime::currentDateTime();//获取结束时间

	//计算时间差
    uint stime = this->get_server_time.toTime_t();
    uint etime = this->stop_server_time.toTime_t();

	//添加到运行时间，以分钟计算
    this->Duration = float(etime-stime);
	airConditionHost->RearchTargetTemp(this->RoomId);
}

bool AirConditionClient::isRunning()//判断分控机对象是否运行
{
    if(this->work_state==2)
        return true;
    else return false;
}

void AirConditionClient::StopRunning()//分控机停止运行
{
	this->work_state = 1;
	this->TimeSliceTimer->stop();
	updateTimer->stop();
    this->stop_server_time = QDateTime::currentDateTime();//获取结束时间

    //计算时间差
    uint stime = this->get_server_time.toTime_t();
    uint etime = this->stop_server_time.toTime_t();

    //添加到运行时间，以分钟计算
    this->Duration = float(etime-stime);
}

void AirConditionClient::StartRunning()//开始运行
{
	this->work_state = STATE_RUN;
    this->get_server_time = QDateTime::currentDateTime();//获取当前时间
}

void AirConditionClient::SetSleep()//设置状态为休眠
{
	this->work_state = STATE_SLEEP;
	this->priority = -1;
}

void AirConditionClient::DestributeRunTime()//分配运行时间片
{

	this->TimeSliceTimer->start(30 * SECOND);//两分钟触发一次
	updateTimer->start(10 * SECOND);
}

void AirConditionClient::WhenTimeOff()//当时间片到
{
	this->TimeSliceTimer->stop();//停止计时器
	updateTimer->stop();

	airConditionHost->TimeOff(this->RoomId);
}

//更新温度和费用
void AirConditionClient::updateAttribute(){
	qDebug()<<"********************************************";
	qDebug()<<QString("%1号房间，温度更新").arg(RoomId);
	qDebug()<<QString("PreTemp = %1; TargetTemp = %2").arg(PreTemp).arg(TargetTemp);
	qDebug()<<"********************************************";

	//高风速1分钟变化1℃，中风速2分钟变化1℃，低风速3分钟变化1℃
	float deltaTemp = 0;
	switch(FanSpeed){
	case 0:
	{
		deltaTemp = (float)UPDATE_PERIOD / (3 * (float)MINUTE);
		break;
	}
	case 1:
	{
		deltaTemp = (float)UPDATE_PERIOD / (2 * (float)MINUTE);
		break;
	}
	case 2:
	{
		deltaTemp = (float)UPDATE_PERIOD / (1 * (float)MINUTE);
		break;
	}
	}
	if(mode)//制热
		PreTemp += deltaTemp;
	else//制冷
		PreTemp -= deltaTemp;
	//1元/摄氏度
	Fee += deltaTemp * 1;
	TotalFee += deltaTemp * 1;

    qDebug()<<"++++++++++++++++++++++++++++++++";
    qDebug()<<QString("%1 room,total fee is %2").arg(this->RoomId).arg(this->TotalFee);
    qDebug()<<"++++++++++++++++++++++++++++++++";

	float target = TargetTemp;
	if(mode) {
		if(target - PreTemp < 0.1)
			ReachTargetTemperature();
	}
	else {
		if(PreTemp - target < 0.1)
			ReachTargetTemperature();
	}
}

void AirConditionClient::SetPriority()//改变优先级
{
	this->priority = this->FanSpeed + 1;
}
