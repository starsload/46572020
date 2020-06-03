#include "GuestClientSocket.h"

GuestClientSocket::GuestClientSocket(QTcpSocket *s)
{
	this->socket = s;
	connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
}

void GuestClientSocket::readData() {
	emit newPacketArrive(socket);
}
