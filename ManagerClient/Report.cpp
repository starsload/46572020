#include "Report.h"

Report::Report(){}

void Report::MakeReport(const QVector<struct report> &t, QString d) {
	report = t;
	date = d;
}

int Report::CreateReportFile()
{
	QString fileName;
	fileName = QString("%1-报表.csv").arg(date);

	QFile out(fileName);
	if (!out.open(QIODevice::WriteOnly | QIODevice::Text))
	  return 0;
	QTextStream file(&out);

	file.setCodec(QTextCodec::codecForName("gbk"));

	file << QString("房间号,空调开关次数,使用空调时长,总费用,调度次数,详单数, ")
	   << QString("调温次数, 调风次数")
	   << endl;

	for (int i = 0; i < report.size(); i++)   //遍历Report数组，依次输出内容到.scv文件
	{
	  file << "Room " << i + 1 << "," << report[i].SwitchOnoffTime << ","
		   << report[i].ServiceTime << "," << report[i].TotalFee <<","
		   << report[i].ScheduleTime << "," << report[i].DetailRecordNum << ","
		   << report[i].ChangeTempTime << "," <<report[i].ChangeFanSpeedTime << endl;
	}
	return 0;
}
