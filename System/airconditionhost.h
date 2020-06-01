#ifndef AIRCONDITIONHOST_H
#define AIRCONDITIONHOST_H

#include <myserver.h>
#include <QCoreApplication>

class AirConditionHost: public QCoreApplication
{
public:
	AirConditionHost(int argc, char *argv[]);
	~AirConditionHost();
	void PowerOn();
	void setPara();
	void startUp();
private:
	MyServer *server;
};

#endif // AIRCONDITIONHOST_H
