#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	initialPage = new InitialPage(this);
	connect(initialPage, SIGNAL(initialFinish(QString, QString))
			, this, SLOT(initialHandle(QString, QString)));
	initialPage->show();
	initialPage->setModal(true);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::initialHandle(QString address, QString port) {
	socket = new QTcpSocket(this);
	socket->connectToHost(address, port.toInt());
}

//点击“生成账单”
void MainWindow::on_ptn_makeInvoice_clicked()
{
	queryInvoiceInputDialg = new QueryInvoiceInputDialog(this);
	connect(queryInvoiceInputDialg, SIGNAL(queryFinish(int)),
			this, SLOT(queryInputFinish(int)));
	queryInvoiceInputDialg->show();
	queryInvoiceInputDialg->setModal(true);
}

//返回Room_Id之后
void MainWindow::queryInputFinish(int Room_Id)
{
	invoicePage = new InvoicePage(this, Room_Id);
	invoicePage->show();
}

//点击“生成报表”
void MainWindow::on_ptn_makeReport_clicked()
{
	reportPage = new ReportPage(this);
	reportPage->show();
}
