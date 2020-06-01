#include "ChartController.h"

ChartController::ChartController(QThread *parent):
	QThread(parent)
{

}

void ChartController::setSocket(QTcpSocket *s) {
	socket = s;
}

void ChartController::run() {
	while(true) {
		socket->read(buffer, HEAD_LENGTH);
		qDebug()<<buffer;
	}
}
