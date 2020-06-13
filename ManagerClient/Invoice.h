#ifndef INOVICE_H
#define INOVICE_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QTextCodec>

class Invoice {
public:
	int roomId; //房间号

	float totalFee; //总费用

	Invoice();

	void Initial(int RoomId, float totalFee);

	void PrintAsFile(); // 打印成文件
};

#endif // INOVICE_H
