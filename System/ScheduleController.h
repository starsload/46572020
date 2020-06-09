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

class AirConditionHost;
class ScheduleController: public QObject
{
	Q_OBJECT

public:
	ScheduleController(QObject *parent = nullptr);
	~ScheduleController();

	void RequestOn(int RoomId,double CurrentRoomTemp);

	void RequestOff(int RoomId);

	void RequestService(int RoomId);

	void setAirConditionHost(AirConditionHost*);

	void addGuestSocket(QTcpSocket *s);

	void ChangeFanSpeed(int RoomId, int Speed);

	void ChangeTargetTemp(int RoomId, float targetTemp);

private slots:
	void listenToGuestClient(QTcpSocket*);

private:
	QVector<GuestClientSocket*> allSockets; //socket数组
	AirConditionHost *airConditionHost;

	GuestClientSocket* curSocket;

	void sendPacket(QByteArray body);
	void sendJSON(QJsonObject ojson);
	void processPacket(QByteArray body);
};

#endif // SCHEDULECONTROLLER_H
