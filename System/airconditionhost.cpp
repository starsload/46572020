#include "AirConditionHost.h"

AirConditionHost::AirConditionHost(int argc, char *argv[]):
	QCoreApplication(argc, argv)
{
	server = new MyServer();
}

AirConditionHost::~AirConditionHost() {
	delete server;
}

void AirConditionHost::PowerOn() {
	qDebug()<<"请输入需要监听的端口：";
	QTextStream input(stdin);
	quint16 port;
	input >> port;
	if(!server->listen(QHostAddress::Any, port))
	{
		qDebug()<<"服务器监听失败";
		return;
	}
	else
	{
		qDebug()<<"开始监听";
	}
}
