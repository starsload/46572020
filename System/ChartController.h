#ifndef CHARTCONTROLLER_H
#define CHARTCONTROLLER_H

#include <QTcpSocket>
#include <QThread>

class ChartController:public QThread
{
public:
	ChartController(QThread *parent = nullptr);
	void setSocket(QTcpSocket*);

protected:
	virtual void run();

private:
	const static int HEAD_LENGTH = 8;

	QTcpSocket *socket;
	char buffer[HEAD_LENGTH];
};

#endif // CHARTCONTROLLER_H
