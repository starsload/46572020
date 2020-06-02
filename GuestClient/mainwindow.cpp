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
	//QByteArray msg = doc.toJson(QJsonDocument::Compact);
	QByteArray msg = doc.toJson(QJsonDocument::Indented);
	sendPacket(msg);
}

void MainWindow::initialHandle(InitialParameters parameters) {
	socket = new QTcpSocket(this);
	QString address = parameters.address;
	QString port = parameters.port;
	socket->connectToHost(address, port.toInt());
	if(socket->isValid()){
		connect(socket, SIGNAL(readyRead()), this, SLOT(newServerMessage()));
		sendTest();
	}
	else{
		qDebug()<<"套接字连接失败";
	}
}

//接收服务器消息
void MainWindow::newServerMessage(){
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
	}
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
