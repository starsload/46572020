#ifndef WAITLIST_H
#define WAITLIST_H

#include "AirConditionClient.h"

class WaitList
{
public:
	WaitList();
	void Initial();

	void pushACC(AirConditionClient*);

private:
	QVector<AirConditionClient*> queue;
};

#endif // WAITLIST_H
