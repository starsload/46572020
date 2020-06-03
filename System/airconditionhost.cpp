#include "AirConditionHost.h"

AirConditionHost::AirConditionHost(QObject *parent):
	QObject(parent)
{
}

AirConditionHost::~AirConditionHost() {
	delete server;
}

void AirConditionHost::PowerOn() {
	server = new QTcpServer();
	db = new QSqlDatabase();
//	QString dsn = QString::fromLocal8Bit("ACCMS");      //数据源名称
//	db->setHostName("localhost");                       //选择本地主机，127.0.1.1
//	db->setDatabaseName(dsn);                           //设置数据源名称
//	db->setUserName("acc_manager");                     //登录用户
//	db->setPassword("123456");                          //密码
	CreatChartController();
	CreateMonitor();
	CreateSchduleController();
	CreateWaitList();
	CreateServiceList();
	qDebug()<<"请输入ManagerClient的端口：";
	QTextStream input(stdin);
	quint16 port;
	input >> port;
	connect(server, SIGNAL(newConnection()),
			this, SLOT(managerConnectHandle()));
	if(!server->listen(QHostAddress::Any, port))
	{
		qDebug()<<"服务器监听失败";
		return;
	}
	else
	{
		qDebug()<<"开始等待管理员";
	}
}

void AirConditionHost::managerConnectHandle() {
	QTcpSocket *managerSocket = server->nextPendingConnection();
	if(!managerSocket->isOpen())
		qDebug()<<"连接建立失败";
	else
		qDebug()<<"管理员已连接";
	chartConstroller->setSocket(managerSocket);
	disconnect(server, SIGNAL(newConnection()),
			this, SLOT(managerConnectHandle()));
	server->disconnect();
	server->close();
}

returnRequestOn AirConditionHost::CtreatClient(int Room_Id, double realTemp){
	AirConditionClient *client;
	client = new AirConditionClient();
	client->Initialize(Room_Id, mode, defaultTargetTemp, realTemp, defaultFeeRate, defaultFanSpeed, *db);
	waitList->pushACC(client);
	returnRequestOn r;
	r.RoomId = Room_Id;
	r.mode = mode;
	r.curTemp = realTemp;
	r.curFanSpeed = defaultFanSpeed;
	r.totalFee = 0;
	r.targetTemp = defaultTargetTemp;
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
	//system("cls");
	qDebug()<<"请输入GuestClientClient的端口：";
	QTextStream input(stdin);
	quint16 port;
	input >> port;
	connect(server, SIGNAL(newConnection()),
			this, SLOT(guestConnectHndle()));
	if(!server->listen(QHostAddress::Any, port))
	{
		qDebug()<<"服务器监听失败";
		return;
	}
	else{
		qDebug()<<"开始等待顾客";
	}
}

void AirConditionHost::guestConnectHndle(){
	qDebug()<<"有顾客客户端连接";
	QTcpSocket *socket = server->nextPendingConnection();
	scheduleController->addGuestSocket(socket);
}

void AirConditionHost::CreatChartController() {
	chartConstroller = new ChartController(this);
	chartConstroller->setAirConditionHost(this);
}

void AirConditionHost::CreateMonitor(){
	monitor = new Monitor();
}

void AirConditionHost::CreateSchduleController(){
	scheduleController = new ScheduleController(this);
	scheduleController->setAirConditionHost(this);
}

void AirConditionHost::CreateWaitList(){
	waitList = new WaitList();
	waitList->Initial();
}

void AirConditionHost::CreateServiceList(){
	serviceList = new ServiceList();
	serviceList->Initial();
}
