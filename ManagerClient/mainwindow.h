#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <initialpage.h>
#include <queryinvoiceinputdialog.h>
#include <invoicepage.h>
#include <reportpage.h>
#include <QTcpSocket>
#include <SocketConstants.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QTimer>
#include "Invoice.h"
#include "DetailRecords.h"
#include "Report.h"

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
	void queryInputFinish(int);

	void onConnected();

	void on_ptn_makeInvoice_clicked();

	void on_ptn_makeReport_clicked();

private slots:
	void initialHandle(InitialParameters para);

	void newServerMessage();

	void offLine();

	void failToConnectServer();

	void onMonitor();

	void onPrintInvoice();

	void onCreateDR(int);

	void onPrintReport(int, QString);

private:
	Ui::MainWindow *ui; //监视主页面

	InitialPage *initialPage; //初始化页面

	QueryInvoiceInputDialog *queryInvoiceInputDialg; //账单查询页面

	InvoicePage *invoicePage = nullptr; //账单页面

	ReportPage *reportPage = nullptr; //报表页面

	QTcpSocket *socket; //套接字

	InitialParameters parameters;

	QTimer socketConnectTimer;

	const int monitorInterval = 2 * 1000;

	QTimer monitorTimer;

	QMessageBox *msgBox;

	Invoice curInvoice;

	void sendPacket(QByteArray body);

	void sendJSON(QJsonObject ojson);

	void processPacket(QByteArray body);

	QString parseRoomState(QJsonObject);

	void print_DR(QJsonObject);

	void printReport(QJsonObject);
};
#endif // MAINWINDOW_H
