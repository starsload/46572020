#ifndef GUESTCLIENTSOCKET_H
#define GUESTCLIENTSOCKET_H

#include <QTcpSocket>

class GuestClientSocket: public QObject
{
	Q_OBJECT

public:
	GuestClientSocket(QTcpSocket *s);

signals:
	void newPacketArrive(QTcpSocket *socket);

private slots:
	void readData();

private:
	QTcpSocket *socket;
};

#endif // GUESTCLIENTSOCKET_H
