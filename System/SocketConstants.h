#ifndef GUSTSOCKTCONSTANTS_H
#define GUSTSOCKTCONSTANTS_H

#include <QString>

namespace SocketConstants {
//包头
static const int HEAD_LEANGTH = 4;

//JSON tag
static const QString TYPE = "type"; //int
//主机初始化TAG
static const QString DEFAULT_TARGET_TEMP = "default_temp"; //int
static const QString MAX_TARGET_TEMP = "max_temp"; //int
static const QString MIN_TARGET_TEMP = "min_temp"; //int
static const QString WORK_MODE = "mode"; //int 0是制冷，1是制热
static const QString HIGH_FEE_RATE = "high_rate"; //double
static const QString MID_FEE_RATE = "mid_rate"; //double
static const QString LOW_FEE_RATE = "low_rate"; //double
static const QString ROOM_ID = "room id"; //int
static const QString ROOM_REAL_TEMP = "real temp"; //double

//操作TYPE的种类
//ChartController
static const int SET_PARA = 0; //主机初始化
static const int SET_PARA_OK = 1; //主机初始化完成
static const int START_UP = 2; //主机开机
static const int START_UP_OK = 3; //主机开机

//ScheduleController
static const int GUEST_ON = 4; //GuestClient建立连接
};



#endif // GUSTSOCKTCONSTANTS_H
