#ifndef INOVICE_H
#define INOVICE_H

#include "UseDatabase.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Invoice {
public:
	int roomId; //房间号

	float totalFee; //总费用

	Invoice();

	void Initial(int RoomId, float totalFee);

	void PrintAsFile(); // 打印成文件
};

#endif // INOVICE_H
