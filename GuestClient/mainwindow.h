#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <initialpage.h>
#include <QTcpSocket>
#include <SocketConstants.h>
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

private:
	Ui::MainWindow *ui;
	InitialPage *initialPage;
	QTcpSocket * socket;

	int RoomId; //房间号
	int tempThreshold; //阈值：目标温度与实际温度的差值
	double realTemp; //房间实际温度
	double targetTemp; //目标温度

	void sendPacket(QByteArray body);
	void sendJSON(QJsonObject ojson);
	void processPacket(QByteArray body);
	void sendTest();
};
#endif // MAINWINDOW_H
