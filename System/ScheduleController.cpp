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

void ScheduleController::listenToGuestClient(QTcpSocket *socket){
	GuestClientSocket *mysocket;
	for(auto sock : allSockets){
		if(sock->socket == socket){
			mysocket = sock;
		}
	}
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
		QString out = body;
		qDebug()<<"收到的数据包信息为：\n"<<out;
		//对body进行处理
		processPacket(mysocket, body);
	}
}

void ScheduleController::processPacket(GuestClientSocket *socket, QByteArray body){
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
	case GUEST_ON:
	{
		int room_id = ojson.value(ROOM_ID).toInt();
		double realTemp = ojson.value(ROOM_REAL_TEMP).toDouble();

		break;
	}

	}
}

void ScheduleController::sendJSON(GuestClientSocket *socket, QJsonObject ojson){
	QJsonDocument doc;
	doc.setObject(ojson);
	QByteArray msg = doc.toJson(QJsonDocument::Compact);
	sendPacket(socket, msg);
}

void ScheduleController::sendPacket(GuestClientSocket *s, QByteArray body){
	QByteArray head;
	// 构造头部
	int  length = body.size();
	int len_int = sizeof(length);
	head.resize(len_int);
	memcpy(head.data(), &length, len_int);
	QByteArray packet;
	packet = head + body;
	s->socket->write(packet, packet.size());
}
