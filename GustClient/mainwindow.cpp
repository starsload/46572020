#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	initialPage = new InitialPage(this);
	initialPage->show();
	initialPage->setModal(true);
}

MainWindow::~MainWindow()
{
	delete initialPage;
	delete ui;
}


void MainWindow::on_pushButton_clicked()
{

}
