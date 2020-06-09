#include "monitor.h"

Monitor::Monitor()
{
}

Monitor::~Monitor()
{

}

//TODO: DEBUG
//int *Monitor::SetupMonitor(vector<int> RoomIdList)
//{
//	vector<AirConditionClient> mWaitList_new,mServiceList_new
//	mWaitList_new=mWaitList.getWaitList();
//	mServiceList_new=mServiceList.getServiceList();
//	int w=(int)RoomIdList.size();
//	int *workstate=new int[w];
//	for(int i=0;i<w;i++)
//	{
//		for(int j=0;j<(int)mServiceList_new.size();j++)
//		{
//			if(mServiceList_new[j].RoomId==RoomIdList[i])
//			{
//				workstate[i]=mServiceList_new[j].WorkState;
//			}

//		}
//		for(int j=0;j<(int)mWaitList_new.size();j++)
//		{
//			if(mWaitList_new[j].RoomId==RoomIdList[i])
//			{
//				workstate[i]=mWaitList_new[j].WorkState;
//			}
//		}
//	}
//	return workstate;
//}
