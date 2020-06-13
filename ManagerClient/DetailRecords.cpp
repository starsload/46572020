#include "DetailRecords.h"

DetailRecords::DetailRecords() {
}

void DetailRecords::Initial(int RoomId, QVector<struct DetailRecord> &list){//创建详单
	this->roomId = RoomId;
	this->detail = list;
}

int DetailRecords::PrintAsFile()
{
	QString fileName;
	fileName = QString("房间%1-详单.txt").arg(roomId);

	QFile out(fileName);
	if (!out.open(QIODevice::WriteOnly | QIODevice::Text))
		return 0;

	QTextStream file(&out);

	file.setCodec(QTextCodec::codecForName("utf-8"));

	file << QString("开始时间,停止时间,温度,风速,单价,运行时间,费用") << endl;

	for(int i=0;i<this->detail.size();i++)
	{
		file << detail[i].StartTime << "," << detail[i].EndTime << ","
			 << detail[i].TargetTemp << "," <<detail[i].FanSpeed << ","
			 << detail[i].FeeRate << "," <<detail[i].Duration << ","
			 << detail[i].Fee << endl;
	}
	return 0;
}
