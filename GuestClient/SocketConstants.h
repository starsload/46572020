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
static const QString DEFAULT_SPEED = "default fan speed"; //int 0为低，1为中，2为高

static const QString ROOM_ID = "room id"; //int
static const QString CUR_TEMP = "cur temp"; //double
static const QString CUR_FEE = "cur fee"; //double
static const QString TARGET_TEMP = "target temp"; //double
static const QString CUR_SPEED = "cur speed"; //int 0为低，1为中，2为高
static const QString TOTAL_FEE = "total fee"; //double
static const QString FAN_SPEED = "fan speed"; //int 0为低，1为中，2为高
static const QString LIST_STATE = "list state"; //管理员监控返回的房间状态列表
static const QString ROOM_STATE = "room state"; //房间状态（0关机、1等待、2运行、3回温）
static const QString DETAIL_ARRAY = "detail array"; //QJsonArray，元素为QJsonObject
static const QString START_TIME = "start time"; //起始时间QString
static const QString END_TIME = "end time"; //终止时间QString
static const QString FEE_RATE = "fee rate"; //费率 double
static const QString DURATION = "duration"; //服务时长 double
static const QString DATE = "date"; //报表日期 QString
static const QString REPORT_ARRAY = "report array"; //QJsonArray，元素为QJsonObject
static const QString SWITCH_ON_OFF_TIMES = "switch on off times"; //int
static const QString SERVICE_TIME = "service time"; //double
static const QString SCHEDULE_TIME = "schedule time"; //int
static const QString NUM_DR = "num of DR"; //int
static const QString TIMES_CHANGE_TEMP = "times CT"; //int
static const QString TIMES_CHANGE_SPEED = "times CS"; //int

//操作TYPE的种类
//ChartController
static const int SET_PARA = 0; //主机初始化
static const int SET_PARA_OK = 1; //主机初始化完成
static const int START_UP = 2; //主机开机
static const int START_UP_OK = 3; //主机开机

//ScheduleController
static const int REQUEST_ON = 4; // GuestClient开机
static const int REQUEST_ON_OK = 5; // GuestClient开机返回
static const int CHANGE_FAN_SPEED = 6; // GuestClient改变风速
static const int CHANGE_FAN_SPEED_OK = 7; // GuestClient改变风速返回
static const int CHANGE_TARGET_TEMP = 8; // GuestClient改变温度
static const int CHANGE_TARGET_TEMP_OK = 9; // GuestClient改变风速
static const int REQUEST_SERVICE = 10; // 提出服务请求
static const int REQUEST_SERVICE_OK = 11; // 提出服务请求成功
static const int REQUEST_SERVICE_FAIL = 12; // 提出服务请求拒绝
static const int REQUEST_OFF = 13; // 关机请求
static const int REQUEST_OFF_OK = 14; // 关机请求返回
static const int REQUEST_FEE = 15; // 查询费用
static const int REQUEST_FEE_OK = 16; // 查询费用返回
static const int STOP_RUNNING = 17; // 达到目标温度，停止工作
static const int STATE_IDLE = 18; // 因为调度，进入空闲状态
static const int STATE_WORK = 19; // 因为调度，进入工作状态
static const int CHECK_ROOM_STATE = 20; // 管理员启动监视各房间空调状态
static const int CHECK_ROOM_STATE_OK = 21; // 返回
static const int CREATE_INVOICE = 22; // 创建账单
static const int CREATE_INVOICE_OK = 23; // 返回
static const int PRINT_INVOICE = 24; // 打印账单
static const int CREATE_DR = 25; // 创建详单
static const int CREATE_DR_OK = 26; // 返回
static const int PRINT_DR = 27; // 打印详单
static const int QUERY_REPORT = 28; // 创建报表
static const int QUERY_REPORT_OK = 29; // 返回
static const int PRINT_REPORT = 30; // 打印报表
};

#endif // GUSTSOCKTCONSTANTS_H
