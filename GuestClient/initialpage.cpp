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
	InitialParameters parameters;
	QString addres;
	QString port;
	parameters.address = this->ui->le_ip_address->text();
	parameters.port = this->ui->le_port->text();
	parameters.tempThreshold = this->ui->temp_threshold->value();
	emit initialFinish(parameters);
	this->destroy();
}
