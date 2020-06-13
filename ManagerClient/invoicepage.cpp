#include "invoicepage.h"
#include "ui_invoicepage.h"

InvoicePage::InvoicePage(QWidget *parent, const Invoice &v) :
	QDialog(parent),
	ui(new Ui::InvoicePage)
{
	ui->setupUi(this);
	invoice = v;
	room_id = invoice.roomId;
	ui->ID->setText(QString::number(invoice.roomId));
	QString t;
	t = QString("%1元").arg(QString::number(invoice.totalFee, 'f', 2));
	ui->lab_totalFee->setText(t);
}

InvoicePage::~InvoicePage()
{
	delete ui;
}

//打印账单
void InvoicePage::on_Print_clicked()
{
	emit PrintInvoice();
	//打印账单
	QMessageBox::information(this, "成功", "成功打印账单");
}

//生成详单
void InvoicePage::on_CreateDR_clicked()
{
	emit PrintDR(room_id);
	//生成并打印详单
}
