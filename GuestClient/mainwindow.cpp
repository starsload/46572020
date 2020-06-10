#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	initialPage = new InitialPage(this);
	connect(initialPage, SIGNAL(initialFinish(InitialParameters))
			, this, SLOT(initialHandle(InitialParameters)));
	initialPage->show();
	initialPage->setModal(true);
	connect(&requestFeeTimer, SIGNAL(timeout()), this, SLOT(requestFee()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::initialHandle(InitialParameters parameters) {
	socket = new QTcpSocket(this);
	QString address = parameters.address;
	QString port = parameters.port;
	this->RoomId = parameters.RoomId;
	this->curTemp = parameters.realTemp;
	this->tempThreshold = parameters.tempThreshold;
	socket->connectToHost(address, port.toInt());
	if(socket->isValid()){
		connect(socket, SIGNAL(readyRead()), this, SLOT(newServerMessage()));
		qDebug()<<"套接字连接成功";
		this->ui->DisplayPreTemp->display(curTemp);
		this->ui->RoomId->setText(QString("%1房间").arg(RoomId));
		this->ui->State->setText("关");
	}
	else{
		qDebug()<<"套接字连接失败";
	}
}

//接收服务器消息
void MainWindow::newServerMessage(){
	using namespace SocketConstants;
	while(socket->bytesAvailable() > 0) {
		QByteArray head;
		QByteArray body;
		head = socket->read(HEAD_LEANGTH);
		if(head.size() == 0){
			continue;
		}
		int length;
		memcpy(&length, head.data(), sizeof (length));
		for(int i = 0; i < length; i++)
			body.append(socket->read(1));
		qDebug()<<"收到的数据包信息为：\n"<<body;
		//对body进行处理
		processPacket(body);
	}
}

//处理收到的数据包
void MainWindow::processPacket(QByteArray body){
	using namespace SocketConstants;
	QJsonParseError e;
	QJsonDocument doc = QJsonDocument::fromJson(body, &e);
	if(e.error != QJsonParseError::NoError) {
		qDebug() << "JSON格式错误";
		return;
	}

	QJsonObject ojson = doc.object();
	int type = ojson.value(TYPE).toInt();
	switch (type) {
	case REQUEST_ON_OK:
	{
		RoomId = ojson.value(ROOM_ID).toInt();
		mode = ojson.value(WORK_MODE).toInt();
		curTemp = ojson.value(CUR_TEMP).toDouble();
		targetTemp = ojson.value(TARGET_TEMP).toDouble();
		curFanSpeed = ojson.value(CUR_SPEED).toInt();
		totalFee = ojson.value(TOTAL_FEE).toDouble();
		curFee = totalFee;
		updateWindow();

		requestService();
		break;
	}
	case CHANGE_FAN_SPEED_OK:
	{
		qDebug()<<"风速改变成功";
		break;
	}
	case CHANGE_TARGET_TEMP_OK:
	{
		qDebug()<<"温度改变成功";
		break;
	}
	case REQUEST_SERVICE_OK:
	{
		qDebug()<<"请求服务成功";
		stopTemperatureSimulation();
		requestFeeTimer.start(requestInterval);
		state = RUN;
		updateWindow();
		break;
	}
	case REQUEST_FEE_OK:
	{
		curTemp = ojson.value(CUR_TEMP).toDouble();
		curFee = ojson.value(CUR_FEE).toDouble();
		totalFee = ojson.value(TOTAL_FEE).toDouble();
		updateWindow();
		break;
	}
	case STOP_RUNNING:
	{
		qDebug()<<"达到目标温度，进入待机状态";
		startTemperatureSimulation();
		state = IDLE;
		curFee = ojson.value(CUR_FEE).toDouble();
		totalFee = ojson.value(TOTAL_FEE).toDouble();
		curTemp = ojson.value(CUR_TEMP).toDouble();
		updateWindow();
		break;
	}
	}
}

void MainWindow::sendJSON(QJsonObject ojson){
	QJsonDocument doc;
	doc.setObject(ojson);
	QByteArray msg = doc.toJson(QJsonDocument::Compact);
	sendPacket(msg);
}

void MainWindow::sendPacket(QByteArray body){
	QByteArray head;
	// 构造头部
	int  length = body.size();
	int len_int = sizeof(length);
	head.resize(len_int);
	memcpy(head.data(), &length, len_int);
	QByteArray packet;
	packet = head + body;
	socket->write(packet, packet.size());
}

//发送调温信息
void MainWindow::changeTemp(){
	qDebug()<<"发送调温信息"<<targetTemp;
	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, CHANGE_TARGET_TEMP);
	ojson.insert(ROOM_ID, RoomId);
	ojson.insert(TARGET_TEMP, targetTemp);
	sendJSON(ojson);
}

//上调温度
void MainWindow::on_TargetTempUp_clicked()
{
	if(tempChangeTimer != nullptr){
		tempChangeTimer->stop();
	}
	else {
		tempChangeTimer = new QTimer();
		tempChangeTimer->setSingleShot(true);
		connect(tempChangeTimer, SIGNAL(timeout()), this, SLOT(changeTemp()));
	}
	targetTemp++;
	this->ui->DisplayTargetTemp->display(targetTemp);
	tempChangeTimer->start(changeInterval);
}

//下调温度
void MainWindow::on_TargetTempDown_clicked()
{
	if(tempChangeTimer != nullptr){
		tempChangeTimer->stop();
	}
	else {
		tempChangeTimer = new QTimer();
		tempChangeTimer->setSingleShot(true);
		connect(tempChangeTimer, SIGNAL(timeout()), this, SLOT(changeTemp()));
	}
	targetTemp--;
	this->ui->DisplayTargetTemp->display(targetTemp);
	tempChangeTimer->start(changeInterval);
}

//调整风速发包
void MainWindow::changeFanSpeed(){
	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, CHANGE_FAN_SPEED);
	ojson.insert(ROOM_ID, RoomId);
	ojson.insert(FAN_SPEED, curFanSpeed);
	sendJSON(ojson);
}

//调整风速按钮
void MainWindow::on_ChangeFanSpeed_clicked()
{
	if(speedChangeTimer != nullptr){
		speedChangeTimer->stop();
	}
	else {
		speedChangeTimer = new QTimer();
		speedChangeTimer->setSingleShot(true);
		connect(speedChangeTimer, SIGNAL(timeout()), this, SLOT(changeFanSpeed()));
	}
	curFanSpeed = (curFanSpeed + 1) % 3;
	updateWindow();
	speedChangeTimer->start(changeInterval);
}

//刷新窗口
void MainWindow::updateWindow(){
	this->ui->DisplayPreTemp->display(QString::number(curTemp, 'f', 1));
	this->ui->RoomId->setText(QString("%1房间").arg(RoomId));
	this->ui->DisplayTargetTemp->display(QString::number(targetTemp, 'f', 1));
	this->ui->TotalFee->setText(QString::number(totalFee, 'f', 2));
	this->ui->Fee->setText(QString::number(curFee, 'f', 2));
	if(mode)
		this->ui->Model->setText("制热");
	else
		this->ui->Model->setText("制冷");
	switch(curFanSpeed) {
	case 0:
		this->ui->FanSpeed->setText("低");
		break;
	case 1:
		this->ui->FanSpeed->setText("中");
		break;
	case 2:
		this->ui->FanSpeed->setText("高");
		break;
	}
	switch (state) {
	case 0:
		this->ui->State->setText("关");
		break;
	case 1:
		this->ui->State->setText("待机");
		break;
	case 2:
		this->ui->State->setText("送风");
		break;
	}
}

//回温程序
void MainWindow::simulTempChange(){
	if(mode){// 制热
		curTemp -= deltaTemp;
	}
	else{// 制冷
		curTemp += deltaTemp;
	}
	updateWindow();
	requestService();
}

//提服务请求
void MainWindow::requestService(){
	bool flag = false;
	if(mode){// 制热
		if(targetTemp - curTemp > (double) tempThreshold){
			flag = true;
		}
	}
	else{// 制冷
		if(curTemp - targetTemp > (double) tempThreshold){
			flag = true;
		}
	}

	if(flag){
		qDebug()<<"提出服务请求";
		using namespace SocketConstants;
		QJsonObject ojson;
		ojson.insert(TYPE, REQUEST_SERVICE);
		ojson.insert(ROOM_ID, RoomId);
		ojson.insert(CUR_TEMP, curTemp);
		sendJSON(ojson);
	}
	else
		startTemperatureSimulation();
}

void MainWindow::startTemperatureSimulation(){
	if(!isTempSimulRun){
		qDebug()<<"回温程序启动";
		if(simulTempTimer == nullptr){
			simulTempTimer = new QTimer();
		}
		connect(simulTempTimer, SIGNAL(timeout()), this, SLOT(simulTempChange()));
		simulTempTimer->start(simulTempInterval);
		isTempSimulRun = true;

		requestFeeTimer.stop();
	}
}

void MainWindow::stopTemperatureSimulation(){
	if(isTempSimulRun){
		qDebug()<<"回温程序关闭";
		disconnect(simulTempTimer, SIGNAL(timeout()), this, SLOT(simulTempChange()));
		simulTempTimer->stop();
		isTempSimulRun = false;
	}
}

//开机键点击
void MainWindow::on_SwitchONOff_clicked()
{
	if(state == CLOSE){
		state = IDLE;
		using namespace SocketConstants;
		QJsonObject ojson;
		ojson.insert(TYPE, REQUEST_ON);
		ojson.insert(ROOM_ID, RoomId);
		ojson.insert(CUR_TEMP, curTemp);
		sendJSON(ojson);
	}
	else{
		state = CLOSE;
		using namespace SocketConstants;
		QJsonObject ojson;
		ojson.insert(TYPE, REQUEST_OFF);
		ojson.insert(ROOM_ID, RoomId);
		startTemperatureSimulation();
	}
	updateWindow();
}

//每间隔10s查询一次
void MainWindow::requestFee(){
	using namespace SocketConstants;
	QJsonObject ojson;
	ojson.insert(TYPE, REQUEST_FEE);
	ojson.insert(ROOM_ID, RoomId);
	sendJSON(ojson);
}
