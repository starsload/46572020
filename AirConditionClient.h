#ifndef AIRCONDITIONCLIENT_H
#define AIRCONDITIONCLIENT_H

#include<vector>
#include<QSqlDatabase>
#include <QDebug>
#include <QtCore/qmath.h>
#include <QFileInfo>
#include <QVector>
#include <QTimer>
using namespace std;

class AirConditionClient
{
private:
        int RoomId;//房间号
        int work_state;//工作状态,0休眠，1等待，2运行
        int mode;//工作模式
        float TargetTemp;//目标温度
        float PreTemp;//当前温度
        float FeeRate;//费率
        float Fee;//当前费用
        float TotalFee;//总费用
        int FanSpeed;//风速
        int priority;//优先级
        float Duration;//使用时长
        QString get_server_time;//获得服务时间戳
        QString stop_server_time;//停止服务时间戳
		QTimer *timer;

private slots:
		void update();


public:
//		AirConditionClient(QObject *parent = nullptr);
		AirConditionClient();
        ~AirConditionClient();

		AirConditionClient(const AirConditionClient& host);

                int GetRoomId(); int Getwork_state(); int Getmode(); float GetTargetTemp(); float GetPreTemp();
                float GetFeeRate(); float GetFee(); float GetTotalFee(); int GetFanSpeed(); int Getpriority(); float GetDuration();
        QString Getget_server_time(); QString Getstop_server_time();
        void Initialize(int RoomId,int mode,int TargetTemp,int PreTemp,float FeeRate,int FanSpeed,QSqlDatabase db);//初始化
        void SetSpeed(int FanSpeed);//调节风速
        void SetTargetTemp(int TargetTemp);//调节温度
        int GetState();//获取分控机运行状态 run运行 sleep休眠 ready准备运行
        QVector<float> GetFinalState();//获取分控机关机时费用和运行时间
        void ReachTargetTemperature();//到达目标温度
        QVector<int> TimeOff();//时间片到
        int GetPriority();//获取优先级
        bool isRunning();//是否运行
        void StopRunning();//停止运行
        void StartRunning();//开始运行
        void Delete();//删除分控机
		void DestributeRunTime();
                void Recovery_temp();
};


#endif // AIRCONDITIONCLIENT_H
