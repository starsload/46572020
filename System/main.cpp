#include <QCoreApplication>
#include "AirConditionHost.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	AirConditionHost host;
	host.PowerOn();
	return a.exec();
}
