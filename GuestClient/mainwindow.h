#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <initialpage.h>
#include <QTcpSocket>
#include <SocketConstants.h>
#include <QTimer>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void initialHandle(InitialParameters);

	void onConnected();

	void newServerMessage();

	void on_TargetTempUp_clicked();

	void on_TargetTempDown_clicked();

	void changeTemp();

	void changeFanSpeed();

	void simulTempChange();

	void on_ChangeFanSpeed_clicked();

	void on_SwitchONOff_clicked();

	void requestFee();

	void offLine();

	void failToConnectServer();

private:
	Ui::MainWindow *ui;
	InitialPage *initialPage;
	QTcpSocket * socket;
	QTimer *tempChangeTimer = nullptr;
	QTimer *speedChangeTimer = nullptr;
	QTimer *simulTempTimer = nullptr;
	QTimer requestFeeTimer;

	QTimer socketConnectTimer;

	QMessageBox *msgBox;

	int RoomId = -1; //房间号
	int tempThreshold = -1; //阈值：目标温度与实际温度的差值
	double curTemp = -1; //房间实际温度
	double initTemp = -1; //房间默认的初始温度
	double targetTemp = -1; //目标温度
	int curFanSpeed = -1; //风速
	int mode = -1; //工作模式 0是制冷，1是制热
	double totalFee = -1; //累计总花费
	double curFee = -1; //本次花费

	int state = 0; //状态：0表示关机状态，1表示开机不送风状态，2表示送风状态

	bool isTempSimulRun = false;//回温程序是否启动

	const int changeInterval = 2000; // 修改参数的时间间隔
	const int simulTempInterval = 60000; // 回温系统，每隔60s变化0.5度
	const int requestInterval = 10000; // 每10秒查询一次
	const double deltaTemp = 0.5;

	const int CLOSE = 0;
	const int IDLE = 1;
	const int RUN = 2;
	const int GO_BACK = 3;

	void sendPacket(QByteArray body);
	void sendJSON(QJsonObject ojson);
	void processPacket(QByteArray body);

	void updateWindow();

	void requestService();

	void startTemperatureSimulation();

	void stopTemperatureSimulation();
};
#endif // MAINWINDOW_H
