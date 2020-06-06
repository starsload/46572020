#ifndef REPORT_H
#define REPORT_H

#include <fstream>
#include "UseDatabase.h"

using namespace std;

class Report
{
public:
    QVector<struct report> report;  //报表内容，包含5个struct report

    Report(QString Date,QSqlDatabase db);   //构造函数，参数①Date：报表日期，②db：数据库
    int CreateReportFile(QString Date,QSqlDatabase db);     //生成报表文件，返回值为0表示操作成功
};

#endif // REPORT_H
