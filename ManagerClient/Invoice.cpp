#include "Invoice.h"

Invoice::Invoice() {
}

void Invoice::Initial(int RoomId, float totalFee){
	this->roomId = RoomId;
	this->totalFee = totalFee;
}

void Invoice::PrintAsFile(){
	QString fileName;
	fileName = QString("房间%1-账单.txt").arg(roomId);

	QFile out(fileName);

	if (!out.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream file(&out);

	file.setCodec(QTextCodec::codecForName("utf-8"));

	file << QString("房间") <<roomId
		 << QString("\t总费用：") << totalFee;
	out.close();
}
