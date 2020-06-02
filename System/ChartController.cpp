#include "ChartController.h"
#include "AirConditionHost.h"

ChartController::ChartController(QObject *parent):
	QObject(parent)
{
}

void ChartController::setAirConditionHost(AirConditionHost *host){
	airConditionHost = host;
}

void ChartController::setSocket(QTcpSocket *s) {
	socket = s;
	connect(socket, SIGNAL(readyRead()),
			this, SLOT(listenToManagerClient()));
	QByteArray msg = "powerON OK";
	sendPacket(msg);
}

void ChartController::listenToManagerClient(){
	qDebug()<<"收到新消息ManagerClient，地址为："<<socket->peerAddress()
		  <<"端口为："<<socket->peerPort();
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
		qDebug()<<"收到的消息为：\n"<<body;
		//对body进行处理
		if (body == "setPara") {
			airConditionHost->setPara();
			QByteArray msg = "setPara OK";
			sendPacket(msg);
		}
		else if (body == "StartUp") {
			airConditionHost->startUp();
			QByteArray msg = "StartUp OK";
			sendPacket(msg);
		}
	}
}

void ChartController::sendPacket(QByteArray body){
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
