#ifndef MYTHREAD_H
#define MYTHREAD_H
#include<QThread>
#include<QTcpSocket>
#include<QDebug>

const int MAX_SIZE=1024;//每次传的数据包的大小，单位：字节

class MyThread: public QThread
{
public:
	explicit MyThread(qintptr socketDescriptor, QThread *parent = nullptr);

protected:
	virtual void run();

private:
	QTcpSocket *socket;
	qintptr socketDescriptor;
	char buffer[MAX_SIZE];

	slots void ReadData();

};

#endif // MYTHREAD_H
