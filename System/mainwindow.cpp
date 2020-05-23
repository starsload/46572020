#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

}

MainWindow::~MainWindow()
{
	delete initialPage;
	delete ui;
}

void MainWindow::on_pushButton_clicked()
{
	this->hide();
	initialPage = new InitialPage(this);
	initialPage->show();
}
