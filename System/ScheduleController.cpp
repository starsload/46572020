#include "ScheduleController.h"

ScheduleController::ScheduleController(QObject *parent):
	QObject(parent)
{

}

ScheduleController::~ScheduleController()
{
}

void ScheduleController::setAirConditionHost(AirConditionHost* s){
	airConditionHost = s;
}

void ScheduleController::addGuestSocket(QTcpSocket *s){
	GuestClientSocket *mySocket;
	mySocket = new GuestClientSocket(s);
	qDebug()<<"对端的地址为："<<s->peerAddress()<<"端口为"<<s->peerPort();
	allSockets.push_back(mySocket);
	connect(mySocket, SIGNAL(newPacketArrive(QTcpSocket*)),
			this, SLOT(listenToGuestClient(QTcpSocket*)));
}

//QJsonObject ojson;
//ojson.insert("type", "hello guest client");
//ojson.insert("num", 34);
//QJsonArray tmpArray;
//tmpArray.push_back(true);
//tmpArray.push_back(78);
//QJsonObject tmp;
//tmp.insert("msg", "hellp world");
//tmpArray.push_back(tmp);
//ojson.insert("array", tmpArray);

void ScheduleController::listenToGuestClient(QTcpSocket *socket){
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
		QString out = body;
		qDebug()<<"收到的数据包信息为：\n"<<out;
		//对body进行处理

		QJsonParseError e;
		QJsonDocument doc = QJsonDocument::fromJson(body, &e);
		if(e.error != QJsonParseError::NoError) {
			qDebug() << "JSON格式错误";
			continue;
		}
		else {
			qDebug() << "JSON格式正确";
			QJsonObject ojson = doc.object();
			QString type = ojson.value("type").toString();
			int num = ojson.value("num").toInt();
			QJsonArray array = ojson.value("array").toArray();
			qDebug()<<type<<num<<array;
		}
	}
}

void ScheduleController::sendPacket(QTcpSocket *socket, QByteArray body){
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
