#ifndef DETAILRECORD_H
#define DETAILRECORD_H

#include "UseDatabase.h"
#include <QFile>
#include <QTextCodec>

class DetailRecords {
public:
    int roomId;     //房间号
    QVector<struct DetailRecord> detail; //详单内容
    float totalFee;                      //总费用

	DetailRecords();

	void Initial(int RoomId, QVector<struct DetailRecord>&);

	int PrintAsFile();//打印详单

};



#endif // DETAILRECORD_H
