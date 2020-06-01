#include "invoicepage.h"
#include "ui_invoicepage.h"

InvoicePage::InvoicePage(QWidget *parent, int Room_Id) :
	QDialog(parent),
	ui(new Ui::InvoicePage)
{
	ui->setupUi(this);
	ui->ID->setText(QString::number(Room_Id));
}

InvoicePage::~InvoicePage()
{
	delete ui;
}

//打印账单
void InvoicePage::on_Print_clicked()
{
	//打印账单
	QMessageBox::information(this, "成功", "成功打印账单");
}

//生成详单
void InvoicePage::on_CreateDR_clicked()
{
	//生成并打印详单
	QMessageBox::information(this, "成功", "成功生成并打印详单");
}
