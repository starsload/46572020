/*
 *功能：生成报表对象，打印报表文件
 *调用时机：经理发出相关指令，主机收集相关数据
*/

#ifndef REPORT_H
#define REPORT_H

#include <fstream>
#include "UseDatabase.h"

using namespace std;

class Report
{
public:
    QVector<struct report> report;

    Report(QString Date,QSqlDatabase db);   //构造函数
    int CreateReportFile(QString Date,QSqlDatabase db);     //生成报表文件
};

#endif // REPORT_H
