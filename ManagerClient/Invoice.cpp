#include "Invoice.h"

Invoice::Invoice() {
}

void Invoice::Initial(int RoomId, float totalFee){
	this->roomId = RoomId;
	this->totalFee = totalFee;
}

void Invoice::PrintAsFile(){
	QString fileName = "房间" + QString(roomId) + "-账单.txt";
	QFile out(fileName);

	if (!out.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream file(&out);

	file << "房间" <<roomId << "\t总费用："<< totalFee;
	out.close();
}
