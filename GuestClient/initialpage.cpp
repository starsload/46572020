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
	parameters.realTemp = this->ui->temp_real->value();
	QString roomid = this->ui->le_room_id->text();
	if (isNum(roomid)){
		parameters.RoomId = roomid.toInt();
	}
	else {
		qDebug()<<"出错！room_id必须为数字";
	}
	emit initialFinish(parameters);
	this->destroy();
}

bool InitialPage::isNum(QString s){
	for(QChar c : s) {
		if(c < '0' || c > '9')
			return false;
	}
	return true;
}
