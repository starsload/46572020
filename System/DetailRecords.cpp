#include "DetailRecords.h"

DetailRecords::DetailRecords() {
}

void DetailRecords::Initial(int RoomId, QVector<struct DetailRecord> list){//创建详单
    this->roomId = RoomId;
	this->detail = list;
}

int DetailRecords:: PrintAsFile()
{
	QString fileName;
	fileName = QString("房间%1-详单.csv").arg(roomId);

	QFile out(fileName);
	if (!out.open(QIODevice::WriteOnly | QIODevice::Text))
		return 0;

	QTextStream file(&out);

	file.setCodec(QTextCodec::codecForName("utf-8"));

	file << QString("开始时间,停止时间,目标温度,风速,单价(元/℃),运行时间,费用") << endl;

	for(int i=0;i<this->detail.size();i++)
	{
		QString speed;
		switch(detail[i].FanSpeed) {
		case 0:
			speed = "低";
			break;
		case 1:
			speed = "中";
			break;
		case 2:
			speed = "高";
			break;
		}

		file << detail[i].StartTime << "," << detail[i].EndTime << ","
			 << detail[i].TargetTemp << "," << speed << ","
			 << detail[i].FeeRate << "," <<detail[i].Duration << ","
			 << detail[i].Fee << endl;
	}
	return 0;
}
