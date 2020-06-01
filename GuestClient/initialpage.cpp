#include "initialpage.h"
#include "ui_initialpage.h"

InitialPage::InitialPage(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::InitialPage)
{
	ui->setupUi(this);
}

InitialPage::~InitialPage()
{
	delete ui;
}

void InitialPage::on_btn_yes_clicked()
{
	QString addres;
	QString port;
	addres=this->ui->le_ip_address->text();
	port = this->ui->le_port->text();
	emit initialFinish(addres, port);
	this->destroy();
}
