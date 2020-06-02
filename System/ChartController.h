#ifndef CHARTCONTROLLER_H
#define CHARTCONTROLLER_H

#include <QTcpSocket>
#include <QObject>
#include <QHostAddress>

class AirConditionHost;
class ChartController:public QObject
{
	Q_OBJECT

public:
	explicit ChartController(QObject *parent = nullptr);
	void setSocket(QTcpSocket*);
	void setHost(AirConditionHost *host);

private slots:
	void listenToManagerClient();

private:
	const static int HEAD_LENGTH = 20;
	QTcpSocket *socket;
	QByteArray buffer;
	AirConditionHost *airConditionHost;
};

#endif // CHARTCONTROLLER_H
