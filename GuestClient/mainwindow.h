#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <initialpage.h>
#include <QTcpSocket>

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
	void initialHandle(QString address, QString port);

private:
	Ui::MainWindow *ui;
	InitialPage *initialPage;
	QTcpSocket * socket;
};
#endif // MAINWINDOW_H
