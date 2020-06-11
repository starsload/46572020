#ifndef GUESTCLIENTSOCKET_H
#define GUESTCLIENTSOCKET_H

#include <QTcpSocket>

class GuestClientSocket: public QObject
{
	Q_OBJECT

public:
	QTcpSocket *socket;
	int Room_id = -1;
	GuestClientSocket(QTcpSocket *s = nullptr);

	~GuestClientSocket();

signals:
	void newPacketArrive(QTcpSocket *socket);

	void Disconnected(QTcpSocket*);

private slots:
	void readData();

};

#endif // GUESTCLIENTSOCKET_H
