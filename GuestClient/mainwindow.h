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
	void initialHandle(InitialParameters parameters);
	void newServerMessage();
	void on_TargetTempUp_clicked();
	void on_TargetTempDown_clicked();

	void changeTemp();

	void changeFanSpeed();

	void on_ChangeFanSpeed_clicked();

	void on_SwitchONOff_clicked();

private:
	Ui::MainWindow *ui;
	InitialPage *initialPage;
	QTcpSocket * socket;
	QTimer *tempChangeTimer = nullptr;
	QTimer *speedChangeTimer = nullptr;

	int RoomId = -1; //房间号
	int tempThreshold = -1; //阈值：目标温度与实际温度的差值
	double curTemp = -1; //房间实际温度
	double targetTemp = -1; //目标温度
	int curFanSpeed = -1; //风速
	int mode = -1; //工作模式
	double totalFee = -1; //累计总花费
	double curFee = -1; //本次花费

	int state = 0; //状态：0表示关机状态，1表示开机不送风状态，2表示送风状态

	const int changeInterval = 2000; //修改参数的时间间隔

	const int CLOSE = 0;
	const int IDLE = 1;
	const int RUN = 2;

	void sendPacket(QByteArray body);
	void sendJSON(QJsonObject ojson);
	void processPacket(QByteArray body);

	void updateWindow();
};
#endif // MAINWINDOW_H
