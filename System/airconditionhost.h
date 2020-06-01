#ifndef AIRCONDITIONHOST_H
#define AIRCONDITIONHOST_H

#include <QTcpServer>
#include <ChartController.h>
#include <ScheduleController.h>
#include <Monitor.h>
#include <WaitList.h>
#include <ServiceList.h>
#include <QObject>

class AirConditionHost: public QObject
{
	Q_OBJECT

public:
	explicit AirConditionHost(QObject *parent = nullptr);
	~AirConditionHost();

	void PowerOn();
	void setPara();
	void startUp();
	void CreatChartController();
	void CreateMonitor();
	void CreateSchduleController();
	void CreateWaitList();
	void CreateServiceList();

private slots:
	void managerConnectHandle();
	void guestConnectHndle();

private:
	QTcpServer *server;
	ChartController *chartConstroller;
	ScheduleController *scheduleController;
	Monitor *monitor;
	WaitList *waitList;
	ServiceList *serviceList;
};

#endif // AIRCONDITIONHOST_H
