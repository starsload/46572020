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

//点击“确认”
void InitialPage::on_ptn_yes_clicked()
{
	InitialParameters parameters;
	QString addres;
	QString port;
	parameters.address=this->ui->le_ip_address->text();
	parameters.port = this->ui->le_port->text();
	parameters.defaultTargetTemp = this->ui->default_target_temp->value();
	parameters.maxTargetTemp = this->ui->max_target_temp->value();
	parameters.minTargetTemp = this->ui->min_target_temp->value();
	parameters.highFeeRate = this->ui->high_fee_rate->value();
	parameters.middleFeeRate = this->ui->middle_fee_rate->value();
	parameters.lowFeeRate = this->ui->low_fee_rate->value();
	parameters.mode = this->ui->mode->currentIndex(); //0是制冷，1是制热
	emit initialFinish(parameters);
	this->destroy();
}
