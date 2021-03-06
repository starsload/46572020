﻿#include "AirConditionHost.h"
#include "UseDatabase.h"
#include "Invoice.h"
#include "Report.h"

AirConditionHost::AirConditionHost(QObject *parent):
	QObject(parent)
{
}

AirConditionHost::~AirConditionHost() {
	delete server;
}

void AirConditionHost::PowerOn() {
	server = new QTcpServer();

	tmpDB = QSqlDatabase::addDatabase("QODBC");
	db = &tmpDB;
	qDebug()<<"ODBC driver is valid? "<<db->isValid();
	QString dsn = QString::fromLocal8Bit("ACCMS_R");       //数据源名称
	db->setHostName("112.74.57.177");
	db->setDatabaseName(dsn);                            //设置数据源名称
	db->setUserName("sa");                               //登录用户
	db->setPassword("308eWORK");                         //密码
	if (!db->open()){
		qDebug()<<"数据库打开失败";
		qDebug()<<db->lastError().text();
	}

	QDateTime dateTime(QDateTime::currentDateTime());
	Date = dateTime.toString("yyyy-MM-dd");
	qDebug() << Date;
	InsertACCchart(Date,*db);

	CreateWaitList();
	CreateServiceList();
	CreateMonitor();
	CreatChartController();
	CreateSchduleController();
	qDebug()<<"服务器的端口：";
	QTextStream input(stdin);
	input >> port;
	connect(server, SIGNAL(newConnection()),
			this, SLOT(managerConnectHandle()));
	if(!server->listen(QHostAddress::Any, port))
	{
		qDebug()<<"Server listener failed!";
		return;
	}
	else
	{
		qDebug()<<"Wait for the manager:";
	}
}

void AirConditionHost::managerConnectHandle() {
	QTcpSocket *managerSocket = server->nextPendingConnection();
	if(!managerSocket->isOpen())
		qDebug()<<"connect failed";
	else
		qDebug()<<"manager connected";
	chartConstroller->setSocket(managerSocket);
	disconnect(server, SIGNAL(newConnection()),
			this, SLOT(managerConnectHandle()));
	server->disconnect();
	server->close();
}

returnRequestOn AirConditionHost::CtreatClient(int Room_Id, double realTemp){
	AirConditionClient *client;
	returnRequestOn r;
	float totalFee = QueryTotalFee(this->Date,this->Date,Room_Id,*db);

	if(this->waitList->FindACC(Room_Id)){//分控机已经存在
		client = this->waitList->FindACC(Room_Id);
		client->Initialize(totalFee,Room_Id, mode, defaultTargetTemp, realTemp, defaultFeeRate, defaultFanSpeed, *db);
		waitList->PushACC(client);

		r.RoomId = Room_Id;
		r.mode = mode;
		r.curTemp = realTemp;
		r.curFanSpeed = defaultFanSpeed;
		r.totalFee = totalFee;
		r.targetTemp = defaultTargetTemp;
	}
	else {//分控机不存在
		client = new AirConditionClient();
		client->Initialize(totalFee,Room_Id, mode, defaultTargetTemp, realTemp, defaultFeeRate, defaultFanSpeed, *db);
		// 与主机建立关联
		client->setAirConditionHostRelation(this);
		//放入等待队列
		waitList->PushACC(client);

		r.RoomId = Room_Id;
		r.mode = mode;
		r.curTemp = realTemp;
		r.curFanSpeed = defaultFanSpeed;
		r.totalFee = QueryTotalFee(this->Date,this->Date,Room_Id,*db);
		r.targetTemp = defaultTargetTemp;
	}
	return r;
}

void AirConditionHost::setPara(double defaultTargetTemp, double maxTargetTemp, double minTargetTemp,
double highFeeRate, double middleFeeRate, double lowFeeRate, int mode, int speed){
	this->defaultTargetTemp = defaultTargetTemp;
	this->maxTargetTemp = maxTargetTemp;
	this->minTargetTemp = minTargetTemp;
	this->highFeeRate = highFeeRate;
	this->middleFeeRate = middleFeeRate;
	this->lowFeeRate = lowFeeRate;
	this->mode = mode;
	this->defaultFanSpeed = speed;
	switch (speed) {
	case 0:
		this->defaultFeeRate = lowFeeRate;
		break;
	case 1:
		this->defaultFeeRate = middleFeeRate;
		break;
	case 2:
		this->defaultFeeRate = highFeeRate;
		break;
	}
}

void AirConditionHost::startUp() {
	connect(server, SIGNAL(newConnection()),
			this, SLOT(guestConnectHndle()));
	if(!server->listen(QHostAddress::Any, port))
	{
		qDebug()<<"server listen failed";
		return;
	}
	else{
		qDebug()<<"wait for guest";
	}
}

void AirConditionHost::guestConnectHndle() {
	qDebug()<<"有顾客客户端连接";
	QTcpSocket *socket = server->nextPendingConnection();
	scheduleController->addGuestSocket(socket);
}

void AirConditionHost::CreatChartController() {
	chartConstroller = new ChartController(this);
	chartConstroller->setAirConditionHostRelation(this);
	chartConstroller->setMonitorRelation(monitor);
}

void AirConditionHost::CreateMonitor(){
	monitor = new Monitor();
	monitor->setServiceListRelation(serviceList);
	monitor->setWaitListRelation(waitList);
}

void AirConditionHost::CreateSchduleController(){
	scheduleController = new ScheduleController(this);
	scheduleController->setAirConditionHost(this);
	scheduleController->setMonitorRelation(monitor);
}

void AirConditionHost::CreateWaitList(){
	waitList = new WaitList();
	waitList->Initial();
}

void AirConditionHost::CreateServiceList(){
	serviceList = new ServiceList();
	serviceList->Initial();
}

int AirConditionHost::ChangeTargetTemp(int RoomID,float Temp)//设置温度 先在队列里面找 再去设置
{
	AirConditionClient* mclient;
	if (mclient = waitList->FindACC(RoomID)) {
		mclient->SetTargetTemp(Temp);
	}
	else if (mclient = serviceList->FindACC(RoomID)) {
		mclient->SetTargetTemp(Temp);
	}
	UpdateChangeTempTime(RoomID,this->Date,*db);//db操作
	return 0;
}

int AirConditionHost:: ChangeFanSpeed(int RoomID,float Speed)//改变风速
{
	AirConditionClient* mclient;
	AirConditionClient* mVictimclient;
	AirConditionClient* mFrontclient;
	if (mclient = waitList->FindACC(RoomID))//在等待队列
	{
		if(mclient->GetState() == AirConditionClient::STATE_SLEEP)//休眠
			mclient->SetSpeed(Speed);
		else {//等待状态
			mclient->SetSpeed(Speed);
			if (mclient->GetPriority() > serviceList->GetMinPriority())//C:优先级大于服务队列最小的
			{
				//选择并取出牺牲者
				mVictimclient = serviceList->GetAndPopVictim();//返回一个拷贝对象
				serviceList->PopACC(mVictimclient->GetRoomId());
				waitList->PushACC(mVictimclient);
				mVictimclient->StopRunning();
				scheduleController->SendIdleMsg(mVictimclient->GetRoomId());
				//中断服务，产生详单
				InsertUseData(mVictimclient->GetRoomId(),mVictimclient->Getget_server_time(),
							  mVictimclient->Getstop_server_time(),mVictimclient->GetTargetTemp(),
							  mVictimclient->GetFanSpeed(),mVictimclient->GetFeeRate(),
							  mVictimclient->GetDuration(),mVictimclient->GetFee(),*db);
				UpdateServiceTime(mVictimclient->GetRoomId(),mVictimclient->GetDuration(),this->Date,*db);
                UpdateTotalFee(mVictimclient->GetRoomId(),mVictimclient->GetTotalFee(),this->Date,*db);
				UpdateDetailRecordNum(mVictimclient->GetRoomId(),this->Date,*db);//一次详单 四件套
				UpdateChangeScheduleTime(mVictimclient->GetRoomId(),this->Date,*db);//db操作 发生调度

				//将该分控机移入
				mclient=waitList->PopACC(mclient->GetRoomId());//返回一个拷贝对象
				serviceList->PushACC(mclient);
				mclient->SetSpeed(Speed);
				mclient->StartRunning();
				mclient->DestributeRunTime();
				scheduleController->SendWorkMsg(mclient->GetRoomId());
				//风速变化，产生详单
				InsertUseData(mclient->GetRoomId(),mclient->Getget_server_time(),
							  mclient->Getstop_server_time(),mclient->GetTargetTemp(),
							  mclient->GetFanSpeed(),mclient->GetFeeRate(),
							  mclient->GetDuration(),mclient->GetFee(),*db);
				UpdateServiceTime(mclient->GetRoomId(),mclient->GetDuration(),this->Date,*db);
                UpdateTotalFee(mclient->GetRoomId(),mclient->GetTotalFee(),this->Date,*db);
				UpdateDetailRecordNum(mclient->GetRoomId(),this->Date,*db);//一次详单 四件套
				UpdateChangeScheduleTime(mclient->GetRoomId(),this->Date,*db);//db操作 发生调度
			}
		}
	}
	else if (mclient = serviceList->FindACC(RoomID))//在服务队列
	{
		if(waitList->ReadyNum() == 0)//E:等待队列没有等待运行的分控机
			{
            mclient->SetSpeed(Speed);
			InsertUseData(mclient->GetRoomId(),mclient->Getget_server_time(),
						  mclient->Getstop_server_time(),mclient->GetTargetTemp(),
						  mclient->GetFanSpeed(),mclient->GetFeeRate(),
						  mclient->GetDuration(),mclient->GetFee(),*db);
			UpdateServiceTime(mclient->GetRoomId(),mclient->GetDuration(),this->Date,*db);
            UpdateTotalFee(mclient->GetRoomId(),mclient->GetTotalFee(),this->Date,*db);
			UpdateDetailRecordNum(mclient->GetRoomId(),this->Date,*db);//一次详单 四件套

			mclient->StartRunning();

		}
		else {
			mclient->SetSpeed(Speed);//设置风速改变优先级
			if(mclient->GetPriority() < waitList->GetMaxPriority()) {//B:等待队列有等待运行的分控机且优先级高

				//将牺牲者移出
				serviceList->PopACC(RoomID);//返回值是类的拷贝
				waitList->PushACC(mclient);
				mclient->StopRunning();
				scheduleController->SendIdleMsg(mclient->GetRoomId());
				//中断服务，产生详单
				InsertUseData(mclient->GetRoomId(),mclient->Getget_server_time(),
							  mclient->Getstop_server_time(),mclient->GetTargetTemp(),
							  mclient->GetFanSpeed(),mclient->GetFeeRate(),
							  mclient->GetDuration(),mclient->GetFee(),*db);
				UpdateServiceTime(mclient->GetRoomId(),mclient->GetDuration(),this->Date,*db);
                UpdateTotalFee(mclient->GetRoomId(),mclient->GetTotalFee(),this->Date,*db);
				UpdateDetailRecordNum(mclient->GetRoomId(),this->Date,*db);//一次详单 四件套
				UpdateChangeScheduleTime(mclient->GetRoomId(),this->Date,*db);//db操作 发生调度

				//将优先级高的分控机移入
				mFrontclient = waitList->GetAndPopFrontACC();
				serviceList->PushACC(mFrontclient);
				mFrontclient->StartRunning();
				mFrontclient->DestributeRunTime();
				scheduleController->SendWorkMsg(mFrontclient->GetRoomId());

				UpdateChangeScheduleTime(mFrontclient->GetRoomId(),this->Date,*db);//db操作 发生调度

			}
		}
	}
	UpdateChangeFanSpeedTime(RoomID,this->Date,*db);//db操作

	qDebug()<<"=======================================================";
	qDebug()<<QString("%1 room change FanSpeed,schedule").arg(RoomID);
	qDebug()<<QString("FanSpeed:%1").arg(Speed);
	waitList->debug();
	serviceList->debug();
	qDebug()<<"=======================================================";

	return 0;
}

bool AirConditionHost::RequestService(int RoomId, float PreTemp) {
	AirConditionClient* mclient = waitList->FindACC(RoomId);    //查找房间号对应的client
	mclient->SetPreTemp(PreTemp);

	//将优先级改回
	mclient->SetPriority();

	AirConditionClient* mVictimclient;  //被牺牲的client
	bool flag;
	if (!serviceList->isFull()) //服务队列未满
	{
		waitList->PopACC(RoomId);     //从等待队列弹出client
		serviceList->PushACC(mclient);  //服务队列加入mclient
		mclient->StartRunning();    //mclient开始运行
		mclient->DestributeRunTime();   //给mclient分配时间片
		UpdateChangeScheduleTime(mclient->GetRoomId(),this->Date,*db);//db操作 发生调度

		flag = true;
	}
	else if(mclient->GetPriority()> serviceList->GetMinPriority()) { //mclient的优先级大于服务队列中的最小优先级
		mVictimclient = serviceList->GetAndPopVictim();//找到牺牲者
		waitList->PushACC(mVictimclient);   //将牺牲者加入等待队列
		mVictimclient->StopRunning();   //牺牲者停止服务
		//中断服务，产生详单
		InsertUseData(mVictimclient->GetRoomId(),mVictimclient->Getget_server_time(),
					  mVictimclient->Getstop_server_time(),mVictimclient->GetTargetTemp(),
					  mVictimclient->GetFanSpeed(),mVictimclient->GetFeeRate(),
					  mVictimclient->GetDuration(),mVictimclient->GetFee(),*db);
		UpdateServiceTime(mVictimclient->GetRoomId(),mVictimclient->GetDuration(),this->Date,*db);
        UpdateTotalFee(mVictimclient->GetRoomId(),mVictimclient->GetTotalFee(),this->Date,*db);
		UpdateDetailRecordNum(mVictimclient->GetRoomId(),this->Date,*db);//一次详单 四件套
		UpdateChangeScheduleTime(mVictimclient->GetRoomId(),this->Date,*db);//db操作 发生调度

		//修改
		scheduleController->SendIdleMsg(mVictimclient->GetRoomId());

		waitList->PopACC(mclient->GetRoomId());   //从等待队列移出mclient
		serviceList->PushACC(mclient);  //mclient加入服务队列
		mclient->StartRunning();    //mclient开始服务
		mclient->DestributeRunTime();   //mclient分配时间片

		UpdateChangeScheduleTime(mclient->GetRoomId(),this->Date,*db);//db操作 发生调度

		flag = true;
	}
	else {

		flag = false;
	}

	qDebug()<<"=======================================================";
	qDebug()<<QString("%1 room pull RequestService,schedule").arg(RoomId);
	waitList->debug();
	serviceList->debug();
	qDebug()<<"=======================================================";

	return flag;
}

DetailRecords AirConditionHost::MakeDR(int RoomID)//请求数据库 返回详单对象
{
	QDateTime dateTime(QDateTime::currentDateTime());
	QString data_in = dateTime.toString("yyyy-MM-dd");
	QString data_out = dateTime.toString("yyyy-MM-dd");

	DetailRecords records;
	QVector<struct DetailRecord> detail;
	detail = QueryDataInUseData(RoomID, *db);
	records.Initial(RoomID, detail);
	return records;
}

void AirConditionHost::RequestPrintReport(Report r){
	r.CreateReportFile();
}

Report AirConditionHost::RequestReport(QString date)//请求数据库 返回报表对象
{
	Report mReport;
	QVector<struct report> t = QueryDataInACCchart(date,*db);
	mReport.MakeReport(t, date);
	return mReport;
}

Invoice AirConditionHost::MakeAnInvoice(int RoomID)//请求数据库 返回总花费
{
	QDateTime dateTime(QDateTime::currentDateTime());
	QString data_in = dateTime.toString("yyyy-MM-dd");
	QString data_out = dateTime.toString("yyyy-MM-dd");
	float totalFee = QueryTotalFee(data_in, data_out, RoomID, *db);
	Invoice res;
	res.Initial(RoomID, totalFee);
	return res;
}

void AirConditionHost::TurnOff(int RoomId)//关闭指定分控机
{
	AirConditionClient* client = NULL;
	AirConditionClient* temp = NULL;
	float tempFee,tempDuration;
	client = waitList->FindACC(RoomId);
	if(client != NULL){//在等待队列
		client->StopRunning();
		tempFee = client->GetFee();
		tempDuration = client->GetDuration();

		waitList->PopACC(RoomId);

	}
	else {//在服务队列
		//将分控机从服务队列移出并保存参数
		client = serviceList->FindACC(RoomId);
		if(client != NULL){
			client->StopRunning();
			tempFee = client->GetFee();
			tempDuration = client->GetDuration();
			serviceList->PopACC(RoomId);
			UpdateServiceTime(client->GetRoomId(),client->GetDuration(),this->Date,*db);

			//将等待队列中优先级最高的取出
			if(waitList->ReadyNum() > 0){
				temp=waitList->GetAndPopFrontACC();
				serviceList->PushACC(temp);
				temp->StartRunning();
				temp->DestributeRunTime();

				scheduleController->SendWorkMsg(temp->GetRoomId());

				InsertUseData(temp->GetRoomId(),temp->Getget_server_time(),temp->Getstop_server_time(),temp->GetTargetTemp(),
								 temp->GetFanSpeed(),temp->GetFeeRate(),temp->GetDuration(),temp->GetFee(),*db);
				UpdateDetailRecordNum(temp->GetRoomId(),this->Date,*db);
				UpdateServiceTime(temp->GetRoomId(),temp->GetDuration(),this->Date,*db);
                UpdateTotalFee(client->GetRoomId(),client->GetTotalFee(),this->Date,*db);

			}
		}
		else //两个队列都不在，说明之前已经关机过，可能是断连的情况
			return;
	}

	//将状态插入到数据库
	InsertUseData(RoomId,client->Getget_server_time(),client->Getstop_server_time(),
				  client->GetTargetTemp(),client->GetFanSpeed(),client->GetFeeRate(),tempDuration,tempFee,*db);

	UpdateDetailRecordNum(client->GetRoomId(),this->Date,*db);
    UpdateTotalFee(RoomId, client->GetTotalFee(), Date, *db);
	UpdateChangeScheduleTime(RoomId,this->Date,*db);

	UpdateSwitchOnOffTime(RoomId,Date,*db);

	delete client;//删掉分控机
	client = NULL;
}

void AirConditionHost:: TimeOff(int RoomId)//时间片到的调度
{
	AirConditionClient *client,*temp ;
	if(waitList->ReadyNum() > 0) {//等待队列不为空
		float max = this->waitList->GetMaxPriority();//找到等待队列中最高优先级的分控机的优先级

		if(max<this->serviceList->FindACC(RoomId)->Getpriority())//时间片到达的分控机优先级更高
		{
			//重新分配时间片
			client = this->serviceList->FindACC(RoomId);
			client->DestributeRunTime();

			InsertUseData(client->GetRoomId(),client->Getget_server_time(),client->Getstop_server_time(),client->GetTargetTemp(),
						  client->GetFanSpeed(),client->GetFeeRate(),client->GetDuration(),client->GetFee(),*db);
			UpdateServiceTime(client->GetRoomId(),client->GetDuration(),this->Date,*db);
            UpdateTotalFee(client->GetRoomId(),client->GetTotalFee(),this->Date,*db);
			UpdateDetailRecordNum(client->GetRoomId(),this->Date,*db);//一次详单 四件套

			client->StartRunning();
		}
		else//等待队列中有优先级更高的分控机存在
		{
		   //将服务队列中的分控机取出
		   client = this->serviceList->FindACC(RoomId);
		   this->serviceList->PopACC(RoomId);
		   this->waitList->PushACC(client);
		   client->StopRunning();
		   scheduleController->SendIdleMsg(client->GetRoomId());
		   //中断服务，产生详单
		   InsertUseData(client->GetRoomId(),client->Getget_server_time(),client->Getstop_server_time(),client->GetTargetTemp(),
						 client->GetFanSpeed(),client->GetFeeRate(),client->GetDuration(),client->GetFee(),*db);
		   UpdateServiceTime(client->GetRoomId(),client->GetDuration(),this->Date,*db);
           UpdateTotalFee(client->GetRoomId(),client->GetTotalFee(),this->Date,*db);
		   UpdateDetailRecordNum(client->GetRoomId(),this->Date,*db);//一次详单 四件套
		   UpdateChangeScheduleTime(RoomId,this->Date,*db);//调度次数加一


		   //将等待队列中的分控机放入服务队列
		   temp = this->waitList->GetAndPopFrontACC();
		   this->serviceList->PushACC(temp);
		   //改变分控机状态并分配时间片
		   temp->StartRunning();
		   temp->DestributeRunTime();
		   scheduleController->SendWorkMsg(temp->GetRoomId());
		}
	}
	else {//等待队列为空
		//重新分配时间片
		client = this->serviceList->FindACC(RoomId);
		client->DestributeRunTime();

		InsertUseData(client->GetRoomId(),client->Getget_server_time(),client->Getstop_server_time(),client->GetTargetTemp(),
					  client->GetFanSpeed(),client->GetFeeRate(),client->GetDuration(),client->GetFee(),*db);
		UpdateServiceTime(client->GetRoomId(),client->GetDuration(),this->Date,*db);
        UpdateTotalFee(client->GetRoomId(),client->GetTotalFee(),this->Date,*db);
		UpdateDetailRecordNum(client->GetRoomId(),this->Date,*db);//一次详单 四件套

		client->StartRunning();
	}

	qDebug()<<"=======================================================";
	qDebug()<<QString("%1 room time off，schedule").arg(RoomId);
	waitList->debug();
	serviceList->debug();
	qDebug()<<"=======================================================";
}

void AirConditionHost::RearchTargetTemp(int RoomId)//到达目标温度调度
{
	qDebug()<<"=======================================================";
	qDebug()<<QString("%1 room reach temp,schedule").arg(RoomId);

	//找出分控机对象
	AirConditionClient *client;
	AirConditionClient *temp;

	//移出服务队列
	client = serviceList->PopACC(RoomId);

	//设置分控机对象参数
	client->StopRunning();
	client->SetSleep();//状态为休眠
	scheduleController->SendStopMsg(RoomId, client->GetFee(), client->GetTotalFee(), client->GetPreTemp());

	//调度部分
	if(waitList->ReadyNum() > 0) // 等待队列不为空发生调度
	{
		//将等待队列最高优先级分控机取出放入服务队列
		temp = waitList->GetAndPopFrontACC();
		serviceList->PushACC(temp);

		//改变分控机状态并分配时间片
		temp->StartRunning();
		temp->DestributeRunTime();
		scheduleController->SendWorkMsg(temp->GetRoomId());
	}

	//将完成服务的分控机移入等待队列
	waitList->PushACC(client);

	//中断服务，产生详单
	InsertUseData(client->GetRoomId(),client->Getget_server_time(),client->Getstop_server_time(),client->GetTargetTemp(),
				  client->GetFanSpeed(),client->GetFeeRate(),client->GetDuration(),client->GetFee(),*db);
	UpdateServiceTime(client->GetRoomId(),client->GetDuration(),this->Date,*db);
    UpdateTotalFee(client->GetRoomId(),client->GetTotalFee(),this->Date,*db);
	UpdateDetailRecordNum(client->GetRoomId(),this->Date,*db);//一次详单 四件套
	UpdateChangeScheduleTime(RoomId,this->Date,*db);//调度次数加一
}
