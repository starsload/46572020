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
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::sendTest(){
	QJsonObject ojson;
	ojson.insert("type", "hello guest client");
	ojson.insert("num", 34);
	QJsonArray tmpArray;
	tmpArray.push_back(true);
	tmpArray.push_back(78);
	QJsonObject tmp;
	tmp.insert("msg", "hellp world");
	tmpArray.push_back(tmp);
	ojson.insert("array", tmpArray);

	QJsonDocument doc;
	doc.setObject(ojson);
	QByteArray msg = doc.toJson(QJsonDocument::Compact);
	//QByteArray msg = doc.toJson(QJsonDocument::Indented);
	sendPacket(msg);
}

void MainWindow::initialHandle(InitialParameters parameters) {
	socket = new QTcpSocket(this);
	this->RoomId = parameters.RoomId;
	QString address = parameters.address;
	QString port = parameters.port;
	this->realTemp = parameters.realTemp;
	this->tempThreshold = parameters.tempThreshold;
	socket->connectToHost(address, port.toInt());
	if(socket->isValid()){
		connect(socket, SIGNAL(readyRead()), this, SLOT(newServerMessage()));
		using namespace SocketConstants;
		QJsonObject ojson;
		ojson.insert(TYPE, GUEST_ON);
		ojson.insert(ROOM_ID, parameters.RoomId);
		ojson.insert(ROOM_REAL_TEMP, parameters.realTemp);
		sendJSON(ojson);
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
