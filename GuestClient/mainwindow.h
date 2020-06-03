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

	void sendPacket(QByteArray body);

	void sendTest();
};
#endif // MAINWINDOW_H
