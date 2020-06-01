#include"myserver.h"

MyServer::MyServer(QObject *parent):
	QTcpServer (parent)
{
}

//void MyServer::incomingConnection(qintptr socketDescriptor)
//{
//	qDebug()<<"有新连接！"<<socketDescriptor;
//	MyThread *thread = new MyThread(socketDescriptor, nullptr);
//	thread->start();
//	connect(thread, &MyThread::finished, [&]{qDebug() << "thread is over";});
//	connect(thread, &MyThread::finished, &MyThread::deleteLater);
//}
