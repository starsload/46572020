#include "report.h"

Report::Report(QString Date, QSqlDatabase db)
{
  this->report = QVector<struct Report> QueryDataInACCchart(date, db);
}

int Report::CreateReportFile(QString Date, QSqlDatabase db)
{
  this->report = QVector<struct Report> QueryDataInACCchart(date, db);  //查询数据库获取报表内容

  Qstring fileName = Date + "Report.scv";
  fstream file(fileName, ios::out | ios::trunc);     //打开报表文件

  file << "房间号,空调开关次数,使用空调时长,总费用,调度次数,详单数, "
          "调温次数, 调风次数"
       << endl;

  for (int i = 0; i < report.size(); i++)   //遍历Report数组，依次输出内容到.scv文件
  {
      file << "Room " << i + 1 << "," << report[i].SwitchOnoffTime << ","
           << report[i].ServiceTime << "," << report[i].TotalFee <<","
           << report[i].ScheduleTime << "," << report[i].DetailRecordNum << ","
           << report[i].ChangeTempTime << "," <<report[i].ChangeFanSpeedTime << endl;
  }
  file.close();
}
