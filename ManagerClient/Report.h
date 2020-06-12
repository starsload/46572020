#ifndef REPORT_H
#define REPORT_H

#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QIODevice>

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

class Report
{
public:
	QVector<struct report> report;  //报表内容，包含5个struct report

	Report();

	void MakeReport(const QVector<struct report> &t, QString d);

	int CreateReportFile();//生成报表文件，返回值为0表示操作成功

	QString date;
};

#endif // REPORT_H
