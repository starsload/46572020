#include "DetailRecords.h"
#include "UseDatabase.h"
#include <QString>
#include <fstream>
#include <string>
#include <QFile>
#include <QTextStream>
#include <QIODevice>

DetailRecords::DetailRecords()
{
}

void DetailRecords::Initial(int RoomId, QSqlDatabase db){//创建详单
    this->roomId = RoomId;
    this->detail = QueryDataInUseData(RoomId,db);
}

int DetailRecords:: PrintRDR(int roomId)
{
    QString fileName = "房间" + QString(roomId) + "-详单.scv";  //文件名：房间号-详单.scv
//    ofstream file(fileName, ios::out );  //打开文件

    QFile out(fileName);
    if (!out.open(QIODevice::WriteOnly | QIODevice::Text))
        return 0;
    QTextStream file(&out);

    file << "开始时间,停止时间,温度,风速,单价,运行时间,费用" << endl;

    for(int i=0;i<this->detail.size();i++)
    {
        file << detail[i].StartTime << "," << detail[i].EndTime << ","
             << detail[i].TargetTemp << "," <<detail[i].FanSpeed << ","
             << detail[i].FeeRate << "," <<detail[i].Duration << ","
             << detail[i].Fee <<endl;
    }
    return 0;
}
