#include "WaitList.h"

WaitList::WaitList()
{

}

void WaitList::Initial(){

}

void WaitList::pushACC(AirConditionClient *ACC){
	queue.push_back(ACC);
}
