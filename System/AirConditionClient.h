#ifndef AIRCONDITIONCLIENT_H
#define AIRCONDITIONCLIENT_H

#include<vector>
#include<QSqlDatabase>
#include <QDebug>
#include <QtCore/qmath.h>
#include <QFileInfo>
#include <QVector>
#include <QTimer>
#include <time.h>
#include <QTime>
#include <QDateTime>

class AirConditionHost;

class AirConditionClient: public QObject{
		Q_OBJECT

private:
        int RoomId;//房间号

        int work_state;//工作状态,0休眠，1等待，2运行

        int mode;//工作模式

        int TargetTemp;//目标温度

		float PreTemp;//当前温度

        float FeeRate;//费率

        float Fee;//当前费用

        float TotalFee;//总费用

		int FanSpeed; //风速

		int priority; //优先级

        int Duration;//使用时长

        QDateTime get_server_time;//获得服务时间戳

        QDateTime stop_server_time;//停止服务时间戳

		QTimer *TimeSliceTimer;//用于时间片的计时器

		QTimer *updateTimer; //被服务时，更新温度、费用的计时器
		const int SECOND = 1000;
		const int UPDATE_PERIOD = 10 * SECOND; //更新周期
		const int MINUTE = 60 * SECOND;

		AirConditionHost *airConditionHost;

private slots:
        void WhenTimeOff();

		void updateAttribute();

public:
		static const int STATE_SLEEP = 0;
		static const int STATE_WAIT = 1;
		static const int STATE_RUN = 2;

        AirConditionClient();
        ~AirConditionClient();
		int GetRoomId();

		int Getmode();
		int GetTargetTemp();
		float GetPreTemp();
		float GetFeeRate();
		float GetFee();
		float GetTotalFee();
		int GetFanSpeed();
		int Getpriority();
        float GetDuration();
		QString Getget_server_time();
		QString Getstop_server_time();
        void Initialize(float totalFee, int RoomId,int mode,int TargetTemp,int PreTemp,
						float FeeRate,int FanSpeed,QSqlDatabase db);//初始化

		void SetSpeed(int FanSpeed);//调节风速

		void SetTargetTemp(int TargetTemp);//调节温度

		void SetPreTemp(float temp);// 修改当前温度

		int GetState();//获取分控机运行状态 run运行 sleep休眠 wait准备运行

        QVector<float> GetFinalState();//获取分控机关机时费用和运行时间

        void ReachTargetTemperature();//到达目标温度

        int GetPriority();//获取优先级

        bool isRunning();//是否运行

        void StopRunning();//停止运行

        void StartRunning();//开始运行

        void DestributeRunTime();//分配时间片

        void SetSleep();//设置状态为休眠

		void setAirConditionHostRelation(AirConditionHost*);

		void SetPriority();
};


#endif // AIRCONDITIONCLIENT_H
