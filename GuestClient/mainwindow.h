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

private:
	Ui::MainWindow *ui;
	InitialPage *initialPage;
	QTcpSocket * socket;
	QTimer *tempChangeTimer = nullptr;

	int RoomId; //房间号
	int tempThreshold; //阈值：目标温度与实际温度的差值
	double curTemp; //房间实际温度
	double targetTemp; //目标温度
	int curFanSpeed; //风速
	int mode; //工作模式
	double totalFee; //累计总花费
	double curFee; //本次花费

	const int changeInterval = 2000; //修改参数的时间间隔

	void sendPacket(QByteArray body);
	void sendJSON(QJsonObject ojson);
	void processPacket(QByteArray body);
	void sendTest();
};
#endif // MAINWINDOW_H
