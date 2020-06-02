#include "ChartController.h"
#include "AirConditionHost.h"

ChartController::ChartController(QObject *parent):
	QObject(parent)
{
}

void ChartController::setHost(AirConditionHost *host){
	airConditionHost = host;
}

void ChartController::setSocket(QTcpSocket *s) {
	socket = s;
	connect(socket, SIGNAL(readyRead()),
			this, SLOT(listenToManagerClient()));
	QByteArray msg = "powerON OK";
	socket->write(msg, msg.size());
	socket->waitForBytesWritten();
}

void ChartController::listenToManagerClient(){
	qDebug()<<"收到新消息ManagerClient，地址为："<<socket->peerAddress()
		  <<"端口为："<<socket->peerPort();
	buffer = socket->read(HEAD_LENGTH);
	qDebug()<<"收到的消息为：\n"<<buffer;
	if (buffer == "setPara") {
		airConditionHost->setPara();
		QByteArray msg = "setPara OK";
		socket->write(msg, msg.size());
	}
	else if (buffer == "StartUp") {
		airConditionHost->startUp();
		QByteArray msg = "StartUp OK";
		socket->write(msg, msg.size());
	}
}
