#include "UseDatabase.h"

void InsertUseData(int RoomId,QString StartTime,QString EndTime,float TargetTemp,int FanSpeed,float FeeRate,
				   float Duration,float Fee,QSqlDatabase db)//UseData表插入语句，参数依次为房间号、服务开始时间、服务结束时间、目标温度、风速、费率、服务时长、费用、数据库
{
	//定义SQL语句
	QString str=QString("use ACCMS "
						"insert into use_data(ROOM_ID,START_TIME,END_TIME,TARGET_TEMP,FAN_SPEED,FEE_RATE,DURATION,FEE) "
						"values(%1,'%2','%3',%4,%5,%6,%7,%8)").arg(RoomId).arg(StartTime).arg(EndTime).arg(TargetTemp).arg(FanSpeed).arg(FeeRate).arg(Duration).arg(Fee);
	//定义查询并关联数据库
	QSqlQuery query(db);
	//执行插入语句
	query.exec(str);
}

QVector<struct DetailRecord> QueryDataInUseData(int RoomId,QSqlDatabase db)//从UseData表中查数据，参数为房间号和数据库
{
	//定义SQL语句
	QString str=QString("use ACCMS "
						"select * from use_data where ROOM_ID=%1").arg(RoomId);
	//定义查询
	QSqlQuery query(db);
	//查询
	query.exec(str);
	//临时存储
	struct DetailRecord temp;
	//存储所有查询项
	QVector<struct DetailRecord> RecordList;
	//依次读出查询结果
	while(query.next()){
		temp.RoomId=query.value(0).toInt();
		temp.StartTime=query.value(1).toString();
		temp.EndTime=query.value(2).toString();
		temp.TargetTemp=query.value(3).toFloat();
		temp.FanSpeed=query.value(4).toInt();
		temp.FeeRate = 1.0;
		temp.Duration=query.value(6).toFloat();
		temp.Fee=query.value(7).toFloat();
		RecordList.append(temp);
	}
	return RecordList;
}

void InsertACCchart(QString Date,QSqlDatabase db)//ACCchart表插入语句，参数依次为日期和数据库
{
    QSqlQuery query(db);
    QString str;
    str=QString("use ACCMS "
                "select count(*) from ACC_chart where Date = '%0' ").arg(Date);


    query.exec(str);
    query.first();

    int count = query.value(0).toInt();
    if(count==0){
		for(int i=1;i<6;i++){
            //定义SQL语句
           QString str1=QString("use ACCMS "
                                "insert into ACC_chart(ROOM_ID,SWITCH_ONOFF_TIME,SERVICE_TIME,"
                                "TOTAL_FEE,SCHEDULE_TIME,DETAILRECORD_NUM,CHANGE_TEMP_TIME,CHANGE_FANSPEED_TIME,Date)"
                                "values(%1,%2,%3,%4,%5,%6,%7,%8,'%9') ").arg(i).arg(0).arg(0.0).arg(0.0).arg(0).arg(0).arg(0).arg(0).arg(Date);
            //定义查询并关联数据库
           QSqlQuery query1(db);
            //执行插入语句
           query1.exec(str1);
        }

    }
}

void UpdateSwitchOnOffTime(int RoomId,QString Date,QSqlDatabase db)//更新开机次数，开机时直接调用即可
{
	//定义并执行SQL语句
	QString str=QString("use ACCMS "
						"select SWITCH_ONOFF_TIME from ACC_chart where ROOM_ID = %1 and Date = '%2'").arg(RoomId).arg(Date);
	QSqlQuery query(db);
	query.exec(str);
	query.first();

	//暂存
	float temp=query.value(0).toInt();
	temp=temp+1;
	str=QString("use ACCMS "
				"update ACC_chart set SWITCH_ONOFF_TIME = %1 where ROOM_ID = %2 and Date = '%3'").arg(temp).arg(RoomId).arg(Date);
	query.exec(str);
}

void UpdateChangeTempTime(int RoomId,QString Date,QSqlDatabase db)//更新变温次数，改变温度时直接调用即可
{
	//定义并执行SQL语句
	QString str=QString("use ACCMS "
						"select CHANGE_TEMP_TIME from ACC_chart where ROOM_ID = %1 and Date = '%2'").arg(RoomId).arg(Date);
	QSqlQuery query(db);
	query.exec(str);
	query.first();
	//暂存
	int temp=query.value(0).toInt();
	temp=temp+1;
	str=QString("use ACCMS "
				"update ACC_chart set CHANGE_TEMP_TIME = %1 where ROOM_ID = %2 and Date = '%3'").arg(temp).arg(RoomId).arg(Date);
	query.exec(str);
}

void UpdateChangeFanSpeedTime(int RoomId,QString Date,QSqlDatabase db)//更新调风次数，调节风速时调用即可
{
	//定义并执行SQL语句
	QString str=QString("use ACCMS "
						"select CHANGE_FANSPEED_TIME from ACC_chart where ROOM_ID = %1 and Date = '%2'").arg(RoomId).arg(Date);
	QSqlQuery query(db);
	query.exec(str);
	query.first();
	//暂存
	int temp=query.value(0).toInt();
	temp=temp+1;
	str=QString("use ACCMS "
				"update ACC_chart set CHANGE_FANSPEED_TIME = %1 where ROOM_ID = %2 and Date = '%3'").arg(temp).arg(RoomId).arg(Date);
	query.exec(str);
}

void UpdateChangeScheduleTime(int RoomId,QString Date,QSqlDatabase db)//更新调度次数，发生调度时调用即可
{
	//定义并执行SQL语句
	QString str=QString("use ACCMS "
						"select SCHEDULE_TIME from ACC_chart where ROOM_ID = %1 and Date = '%2'").arg(RoomId).arg(Date);
	QSqlQuery query(db);
	query.exec(str);
	query.first();
	//暂存
	int temp=query.value(0).toInt();
	temp=temp+1;
	str=QString("use ACCMS "
				"update ACC_chart set SCHEDULE_TIME = %1 where ROOM_ID = %2 and Date = '%3'").arg(temp).arg(RoomId).arg(Date);
	query.exec(str);
}

void UpdateServiceTime(int RoomId,float ServiceTime,QString Date,QSqlDatabase db)//更新服务时间，当分控机从等待队列加入服务队列时计算等待时间并记录
{
	//定义并执行SQL语句
	QString str=QString("use ACCMS "
						"select SERVICE_TIME from ACC_chart where ROOM_ID = %1 and Date = '%2'").arg(RoomId).arg(Date);
	QSqlQuery query(db);
	query.exec(str);
	query.first();
	//暂存
	float temp=query.value(0).toFloat();
	temp=temp+ServiceTime;
	str=QString("use ACCMS "
				"update ACC_chart set SERVICE_TIME = %1 where ROOM_ID = %2 and Date = '%3'").arg(temp).arg(RoomId).arg(Date);
	query.exec(str);
}

void UpdateTotalFee(int RoomId,float Fee,QString Date,QSqlDatabase db)//更新总费用，将每天的费用进行累加
{
	//定义并执行SQL语句
	QString str=QString("use ACCMS "
						"select TOTAL_FEE from ACC_chart where ROOM_ID = %1 and Date = '%2'").arg(RoomId).arg(Date);
	QSqlQuery query(db);
	query.exec(str);
	query.first();
	//暂存
	float temp=query.value(0).toFloat();
	temp=temp+Fee;
	str=QString("use ACCMS "
				"update ACC_chart set TOTAL_FEE = %1 where ROOM_ID = %2 and Date = '%3'").arg(temp).arg(RoomId).arg(Date);
	query.exec(str);
}

void UpdateDetailRecordNum(int RoomId,QString Date,QSqlDatabase db)//更新详单数，产生详单时添加即可
{
	//定义并执行SQL语句
	QString str=QString("use ACCMS "
						"select DETAILRECORD_NUM from ACC_chart where ROOM_ID = %1 and Date = '%2'").arg(RoomId).arg(Date);
	QSqlQuery query(db);
	query.exec(str);
	query.first();
	//暂存
	int temp=query.value(0).toInt();
    temp += 1;
	str=QString("use ACCMS "
				"update ACC_chart set DETAILRECORD_NUM = %1 where ROOM_ID = %2 and Date = '%3'").arg(temp).arg(RoomId).arg(Date);
	query.exec(str);
}

float QueryServiceTime(int RoomId,QSqlDatabase db)//查询服务时间
{
	//定义并执行SQL语句
	QString str=QString("use ACCMS "
						"select SERVICE_TIME from ACC_chart where ROOM_ID = %1").arg(RoomId);
	QSqlQuery query(db);
	query.exec(str);

	//累加
	float TotalTime=0.0;
	while(query.next()){
		TotalTime+=query.value(0).toFloat();
	}
	return TotalTime;
}

QVector<struct report> QueryDataInACCchart(QString Date,QSqlDatabase db)//查询报表内容,参数为日期和数据库
{
	//定义SQL语句
	QString str=QString("use ACCMS "
						"select * from ACC_chart where Date='%1'").arg(Date);
	//定义查询
	QSqlQuery query(db);
	//查询
	query.exec(str);
	//临时存储
	struct report temp;
	//存储所有查询项
	QVector<struct report> ReportList;
	//依次读出查询结果
	while(query.next()){
		temp.RoomId=query.value(0).toInt();
		temp.SwitchOnoffTime=query.value(1).toInt();
		temp.ServiceTime=query.value(2).toFloat();
		temp.TotalFee=query.value(3).toFloat();
		temp.ScheduleTime=query.value(4).toInt();
		temp.DetailRecordNum=query.value(5).toInt();
		temp.ChangeTempTime=query.value(6).toInt();
		temp.ChangeFanSpeedTime=query.value(7).toInt();
		ReportList.append(temp);
	}
	return ReportList;
}

float QueryTotalFee(QString DateIn,QString DateOut,int RoomId,QSqlDatabase db)//查询总费用，参数分别为起始日期、结束日期、房间号和数据库
{
	//定义并执行SQL语句
	QString str=QString("use ACCMS "
						"select TOTAL_FEE from ACC_chart where ROOM_ID=%1 and "
						"Date >= '%2' and Date <= '%3' order by Date").arg(RoomId).arg(DateIn).arg(DateOut);
	QSqlQuery query(db);
	query.exec(str);

	//累加
	float TotalFee=0.0;
	while(query.next()){
		TotalFee+=query.value(0).toFloat();
	}
	return TotalFee;
}
