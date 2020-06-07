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
using namespace std;

class AirConditionClient{
private:
        int RoomId;//房间号
        int work_state;//工作状态,0休眠，1等待，2运行
        int mode;//工作模式
        int TargetTemp;//目标温度
        int PreTemp;//当前温度
        float FeeRate;//费率
        float Fee;//当前费用
        float TotalFee;//总费用
        int FanSpeed;//风速
        int priority;//优先级
        int Duration;//使用时长
        QDateTime get_server_time;//获得服务时间戳
        QDateTime stop_server_time;//停止服务时间戳
        QTimer *timer;//用于计算服务时间

private slots:
        void WhenTimeOff();

public:
        AirConditionClient();
        ~AirConditionClient();
        int GetRoomId(); int Getwork_state(); int Getmode(); int GetTargetTemp(); int GetPreTemp();
        float GetFeeRate(); float GetFee(); float GetTotalFee(); int GetFanSpeed(); int Getpriority(); int GetDuration();
        QDateTime Getget_server_time(); QDateTime Getstop_server_time();
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
        void DestributeRunTime();//分配时间片
        void SetSleep();//设置状态为休眠
};


#endif // AIRCONDITIONCLIENT_H
