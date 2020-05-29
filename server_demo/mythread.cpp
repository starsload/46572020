#include"mythread.h"

MyThread::MyThread(qintptr socketDescriptor, QThread *parent)
	: QThread(parent)
{
	 this->socketDescriptor = socketDescriptor;
}

void MyThread::run()
{
	qDebug()<<"创建线程"<<"current thread:"<<QThread::currentThread();
	socket = new QTcpSocket;
	socket->setSocketDescriptor(socketDescriptor);
	connect(socket, &QTcpSocket::readyRead, this ,&MyThread::ReadData , Qt::DirectConnection);
	exec();//子线程进入循环，进行监听
}

void MyThread::ReadData()
{
	socket->read(buffer, MAX_SIZE);
	qDebug()<<"收到的数据为：";
	qDebug()<<buffer;
}
