#ifndef CHARTCONTROLLER_H
#define CHARTCONTROLLER_H

#include <QTcpSocket>
#include <QObject>
#include <QHostAddress>
#include <SocketConstants.h>

class AirConditionHost;
class ChartController:public QObject
{
	Q_OBJECT

public:
	explicit ChartController(QObject *parent = nullptr);
	void setSocket(QTcpSocket*);
	void setAirConditionHost(AirConditionHost *host);

private slots:
	void listenToManagerClient();

private:
	QTcpSocket *socket;
	AirConditionHost *airConditionHost;

	void sendPacket(QByteArray body);
};

#endif // CHARTCONTROLLER_H
