#ifndef CHARTCONTROLLER_H
#define CHARTCONTROLLER_H

#include <QTcpSocket>
#include <QObject>
#include <QHostAddress>
#include <SocketConstants.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "Monitor.h"

class AirConditionHost;

class ChartController:public QObject
{
	Q_OBJECT

public:
	explicit ChartController(QObject *parent = nullptr);

	void setSocket(QTcpSocket* s);

	void setAirConditionHostRelation(AirConditionHost *host);

	void setMonitorRelation(Monitor *m);

	void RequestOff(int RoomId);

	void setPara(int defaultTargetTemp,
				 int maxTargetTemp,
				 int minTargetTemp,
				 double highFeeRate,
				 double middleFeeRate,
				 double lowFeeRate,
				 int mode,
				 int speed);

	void startUp();

	void CheckRoomState();

private slots:
	void listenToManagerClient();

private:
	QTcpSocket *socket;

	AirConditionHost *airConditionHost;

	Monitor *monitor;

	void sendPacket(QByteArray body);
	void sendJSON(QJsonObject ojson);
	void processPacket(QByteArray body);
};

#endif // CHARTCONTROLLER_H
