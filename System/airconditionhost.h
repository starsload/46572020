#ifndef AIRCONDITIONHOST_H
#define AIRCONDITIONHOST_H

#include <QTcpServer>
#include <ChartController.h>
#include <ScheduleController.h>
#include <Monitor.h>
#include <WaitList.h>
#include <ServiceList.h>
#include <AirConditionClient.h>
#include <QObject>

class returnRequestOn{
public:
	int RoomId;
	double curTemp;
	double targetTemp;
	int curFanSpeed;
	int mode;
	double totalFee;
};

class AirConditionHost: public QObject
{
	Q_OBJECT

public:
	explicit AirConditionHost(QObject *parent = nullptr);
	~AirConditionHost();

	void PowerOn();
	void setPara(double defaultTargetTemp,
				 double maxTargetTemp,
				 double minTargetTemp,
				 double highFeeRate,
				 double middleFeeRate,
				 double lowFeeRate,
				 int mode,
				 int speed);
	void startUp();
	void CreatChartController();
	void CreateMonitor();
	void CreateSchduleController();
	void CreateWaitList();
	void CreateServiceList();

	returnRequestOn CtreatClient(int Room_Id, double realTemp);

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
	QSqlDatabase *db;

	double defaultTargetTemp; //默认目标温度
	double maxTargetTemp; //最高目标温度
	double minTargetTemp; //最低目标温度
	double highFeeRate; //高风速费率
	double middleFeeRate; //中风速费率
	double lowFeeRate; //低风速费率
	double defaultFeeRate; //默认费率
	int defaultFanSpeed; //默认风速：0为低，1为中，2为高
	int mode; //工作模式：0是制冷，1是制热
};

#endif // AIRCONDITIONHOST_H
