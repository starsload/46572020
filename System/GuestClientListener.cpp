#include "GuestClientListener.h"

GuestClientListener::GuestClientListener(QTcpSocket *s, int index, QObject *parent):
	QThread(parent)
{
	this->socket = s;
	this->listenerIndex = index;
	this->isInterrupt = false;
}

void GuestClientListener::run() {
//	socket = new QTcpSocket();
//	socket->setSocketDescriptor(descriptor);
	socket->moveToThread(QThread::currentThread());
	qDebug()<<"Thread"<<QThread::currentThread()
		   <<"开始监听客户端,对端地址："<<socket->peerAddress()
		  <<"端口号："<<socket->peerPort();
	while(!isInterrupt) {
		QByteArray head;
		QByteArray body;
		if(!socket->isValid())
			qDebug()<<"子线程socket无效";
		head = socket->read(HEAD_LEANGTH);
		if(head.size() == 0){
			QThread::sleep(500);
			continue;
		}
		int length = head.toInt();
		for(int i = 0; i < length; i++)
			body.append(socket->read(1));
		QByteArray msg = "hello guest client";
		socket->write(msg, msg.size());
		emit newMessageArrive(listenerIndex, body);
	}
}
