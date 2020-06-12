#include "reportpage.h"
#include "ui_reportpage.h"

ReportPage::ReportPage(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ReportPage)
{
	ui->setupUi(this);
}

ReportPage::~ReportPage()
{
	delete ui;
}

// 产生并打印报表
void ReportPage::on_PrintButton_clicked()
{
	QString id_str = this->ui->ID_input->text();
	int id = id_str.toInt();
	QDateTime dateTime = this->ui->date->dateTime();
	QString date = dateTime.toString("yyyy-MM-dd");

	emit Print_Report(id, date);
}
