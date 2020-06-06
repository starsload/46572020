#ifndef INOVICE_H
#define INOVICE_H

#include "UseDatabase.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Inovice {
public:
    int roomId;     //房间号
    QVector<struct DetailRecord> detail; //详单内容
    float totalFee;                      //总费用

    Inovice(int RoomId,
          QSqlDatabase db); //构造函数，生成详单内容，参数①RoomId：房间号，②db：数据库
    Inovice(QString DateIn, QString DateOut, int RoomId,
          QSqlDatabase db);   //构造函数，获取总费用，参数①DateIn：开始时间，②DateOut：结束时间，③RoomId：房间号，④db：数据库
    int CreateRDRfile(int roomId); //打印详单，返回值为0表示操作成功，参数①roomId：房间号
    int CreateFeeFile(int roomId); //打印账单，返回值为0表示操作成功，参数①roomId：房间号
};

#endif // INOVICE_H
