#ifndef MYSERVER_H
#define MYSERVER_H
#include<QTcpServer>
#include<QTcpSocket>
#include<QDebug>
#include<mythread.h>

class MyServer:public QTcpServer
{
public:
	explicit MyServer(QObject *parent = nullptr);

protected:
	//void incomingConnection(qintptr socketDescriptor) override;//重写这个函数
};

#endif // MYSERVER_H
