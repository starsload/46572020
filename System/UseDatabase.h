#ifndef USEDATABASE_H
#define USEDATABASE_H


#include<QSql>
#include <QDebug>
#include<QSqlError>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QString>
#include<QSqlRecord>
#include<QVector>

struct DetailRecord{//记录详单的结构体
    int RoomId;
    QString StartTime;
    QString EndTime;
    float TargetTemp;
    int FanSpeed;
    float FeeRate;
    float Duration;
    float Fee;
};

struct report{//记录报表的结构体
	int RoomId;
	int SwitchOnoffTime;
	float ServiceTime;
	float TotalFee;
	int ScheduleTime;
	int DetailRecordNum;
	int ChangeTempTime;
	int ChangeFanSpeedTime;
};

void InsertUseData(int RoomId,QString StartTime,QString EndTime,float TargetTemp,int FanSpeed,float FeeRate,
				   float Duration,float Fee,QSqlDatabase db);//UseData表插入语句，参数依次为房间号、服务开始时间、服务结束时间、目标温度、风速、费率、服务时长、费用、数据库

QVector<struct DetailRecord> QueryDataInUseData(int RoomId,QSqlDatabase db);//从UseData表中查详单数据，参数为房间号和数据库

void InsertACCchart(QString Date,QSqlDatabase db);//ACCchart表插入语句，参数依次为日期和数据库

void UpdateSwitchOnOffTime(int RoomId,QString Date,QSqlDatabase db);//更新开机次数，开机时直接调用即可

void UpdateChangeTempTime(int RoomId,QString Date,QSqlDatabase db);//更新变温次数，改变温度时直接调用即可

void UpdateChangeFanSpeedTime(int RoomId,QString Date,QSqlDatabase db);//更新调风次数，调节风速时调用即可

void UpdateChangeScheduleTime(int RoomId,QString Date,QSqlDatabase db);//更新调度次数，发生调度时调用即可

void UpdateServiceTime(int RoomId,float ServiceTime,QString Date,QSqlDatabase db);//更新服务时间，当分控机从等待队列加入服务队列时计算等待时间并记录

void UpdateTotalFee(int RoomId,float Fee,QString Date,QSqlDatabase db);//更新总费用，将每天的费用进行累加


void UpdateDetailRecordNum(int RoomId,QString Date,QSqlDatabase db);//更新详单数，产生详单时添加即可


float QueryServiceTime(int RoomId,QSqlDatabase db);//查询服务时间

QVector<struct report> QueryDataInACCchart(QString Date,QSqlDatabase db);//查询报表内容,参数为日期和数据库

float QueryTotalFee(QString DateIn,QString DateOut,int RoomId,QSqlDatabase db);//查询总费用，参数分别为起始日期、结束日期、房间号和数据库


#endif // USEDATABASE_H
