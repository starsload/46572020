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
	void setAirConditionHost(AirConditionHost*);
	void addGuestSocket(QTcpSocket *s);

private slots:
	void listenToGuestClient(QTcpSocket*);
	void test();

private:
	QVector<GuestClientSocket*> allSockets; //socket数组
	AirConditionHost *airConditionHost;

	void sendPacket(QTcpSocket *socket, QByteArray body);
};

#endif // SCHEDULECONTROLLER_H
