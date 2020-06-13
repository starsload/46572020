#ifndef REPORT_H
#define REPORT_H

#include "UseDatabase.h"
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QTextCodec>

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
