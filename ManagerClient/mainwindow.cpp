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
	//传到后台
	connect(socket, SIGNAL(readyRead()), this, SLOT(newServerMessage()));
}

//接收服务器消息
void MainWindow::newServerMessage(){
	while(socket->bytesAvailable() > 0) {
		QByteArray head;
		QByteArray body;
		head = socket->read(HEAD_LEANGTH);
		if(head.size() == 0){
			continue;
		}
		int length;
		memcpy(&length, head.data(), sizeof (length));
		for(int i = 0; i < length; i++)
			body.append(socket->read(1));
		qDebug()<<"收到服务器的信息为："<<body;
		//对body进行处理
		if (body == "powerON OK") {
			QByteArray msg = "setPara";
			sendPacket(msg);
		}
		else if(body == "setPara OK") {
			QByteArray msg = "StartUp";
			sendPacket(msg);
		}
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

void MainWindow::sendPacket(QByteArray body){
	QByteArray head;
	// 构造头部
	int  length = body.size();
	int len_int = sizeof(length);
	head.resize(len_int);
	memcpy(head.data(), &length, len_int);
	QByteArray packet;
	packet = head + body;
	socket->write(packet, packet.size());
}
