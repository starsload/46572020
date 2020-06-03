#ifndef SERVICELIST_H
#define SERVICELIST_H

#include "AirConditionClient.h"
#include <QVector>

class ServiceList
{
public:
	ServiceList();
	void Initial();

private:
	QVector<AirConditionClient*> queue;
};

#endif // SERVICELIST_H
