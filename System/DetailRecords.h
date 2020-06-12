#ifndef DETAILRECORD_H
#define DETAILRECORD_H
#include "UseDatabase.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class DetailRecords {
public:
    int roomId;     //房间号
    QVector<struct DetailRecord> detail; //详单内容
    float totalFee;                      //总费用

    DetailRecords(); //构造函数，生成详单内容，参数①RoomId：房间号，②db：数据库

    void Initial(int RoomId,QSqlDatabase db); //打印详单，返回值为0表示操作成功，参数①roomId：房间号
    int PrintRDR(int roomId);//打印详单

};



#endif // DETAILRECORD_H
