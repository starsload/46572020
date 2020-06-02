#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	initialPage = new InitialPage(this);
	connect(initialPage, SIGNAL(initialFinish(InitialParameters))
			, this, SLOT(initialHandle(InitialParameters)));
	initialPage->show();
	initialPage->setModal(true);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::initialHandle(InitialParameters parameters) {
	QString address = parameters.address;
	QString port = parameters.port;
	socket = new QTcpSocket(this);
	socket->connectToHost(address, port.toInt());
	connect(socket, SIGNAL(readyRead()), this, SLOT(newServerMessage()));
}

void MainWindow::newServerMessage(){
	buffer = socket->read(HEAD_LENGTH);
	qDebug()<<"收到服务器的信息为："<<buffer;\
	if (buffer == "powerON OK") {
		QByteArray msg = "setPara";
		socket->write(msg, msg.size());
	}
	else if(buffer == "setPara OK") {
		QByteArray msg = "StartUp";
		socket->write(msg, msg.size());
	}
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
