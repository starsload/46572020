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

