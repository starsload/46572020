#ifndef SCHEDULECONTROLLER_H
#define SCHEDULECONTROLLER_H

#include <GuestClientSocket.h>
#include <QVector>
#include <QObject>
#include <QHostAddress>
#include <SocketConstants.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "Monitor.h"

class AirConditionHost;
class ScheduleController: public QObject
{
	Q_OBJECT

public:
	ScheduleController(QObject *parent = nullptr);
	~ScheduleController();

	void setAirConditionHost(AirConditionHost*);

	void setMonitorRelation(Monitor *);

	void RequestOn(int RoomId,double CurrentRoomTemp);

	void RequestOff(int RoomId);

	void RequestFee(int RoomId);

	void RequestService(int RoomId, float curTemp);

	void addGuestSocket(QTcpSocket *s);

	void ChangeFanSpeed(int RoomId, int Speed);

	void ChangeTargetTemp(int RoomId, float targetTemp);

	void SendStopMsg(int RoomId, float fee, float totalFee, float curTemp);

	void SendWorkMsg(int RoomId);

	void SendIdleMsg(int RoomId);

private slots:
	void listenToGuestClient(QTcpSocket*);

	void onDisconnected(QTcpSocket*);

private:
	QVector<GuestClientSocket*> allSockets; //socket数组

	AirConditionHost *airConditionHost;

	Monitor *monitor;

	GuestClientSocket* curSocket;

	void sendPacket(QByteArray body);
	void sendJSON(QJsonObject ojson);
	void processPacket(QByteArray body);
};

#endif // SCHEDULECONTROLLER_H
