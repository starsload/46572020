#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::Widget)
{
	ui->setupUi(this);
	socket = new QTcpSocket();
	socket->connectToHost("127.0.0.1",6666);
}

Widget::~Widget()
{
	delete ui;
}


//发送
void Widget::on_pushButton_clicked()
{
	QString msg;
	msg = ui->lineEdit->text();
	QByteArray bytes = msg.toUtf8();
	socket->write(bytes);
}
