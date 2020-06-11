#include "GuestClientSocket.h"

GuestClientSocket::GuestClientSocket(QTcpSocket *s)
{
	this->socket = s;
	connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
	connect(socket, &QTcpSocket::disconnected,[=](){
		emit Disconnected(socket);
	});
}

GuestClientSocket::~GuestClientSocket(){
	socket->close();
	//delete socket;不能删
}

void GuestClientSocket::readData() {
	emit newPacketArrive(socket);
}
