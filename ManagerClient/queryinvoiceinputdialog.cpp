#include "queryinvoiceinputdialog.h"
#include "ui_queryinvoiceinputdialog.h"

QueryInvoiceInputDialog::QueryInvoiceInputDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::QueryInvoiceInputDialog)
{
	ui->setupUi(this);
}

QueryInvoiceInputDialog::~QueryInvoiceInputDialog()
{
	delete ui;
}

//取消
void QueryInvoiceInputDialog::on_pushButton_2_clicked()
{
	this->destroy();
}

void QueryInvoiceInputDialog::on_pushButton_clicked()
{
	QString id = ui->lineEdit->text();
	int Room_Id = id.toInt();
	this->destroy();
	emit queryFinish(Room_Id);
}
