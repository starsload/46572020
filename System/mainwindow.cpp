#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	this->hide();
	initialPage = new InitialPage(this);
	initialPage->show();
}

MainWindow::~MainWindow()
{
	delete initialPage;
	delete ui;
}

//生成账单
void MainWindow::on_pushButton_clicked()
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
