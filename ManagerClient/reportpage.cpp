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
