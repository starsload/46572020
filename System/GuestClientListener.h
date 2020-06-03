#ifndef GUESTCLIENTLISTENER_H
#define GUESTCLIENTLISTENER_H

#include <QThread>
#include <QTcpSocket>
#include <SocketConstants.h>
#include <QHostAddress>

class GuestClientListener: public QThread
{
	Q_OBJECT
public:
	explicit GuestClientListener(QTcpSocket *s, int index, QObject *parent = nullptr);
	bool isInterrupt;//控制子线程退出

signals:
	void newMessageArrive(int listenerIndex, QByteArray msg);

protected:
	virtual void run();

private:
	qintptr descriptor;
	int listenerIndex;
	QTcpSocket *socket;
};

#endif // GUESTCLIENTLISTENER_H
