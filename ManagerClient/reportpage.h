#ifndef REPORTPAGE_H
#define REPORTPAGE_H

#include <QDialog>

namespace Ui {
class ReportPage;
}

class ReportPage : public QDialog
{
	Q_OBJECT

public:
	explicit ReportPage(QWidget *parent = nullptr);
	~ReportPage();

private:
	Ui::ReportPage *ui;
};

#endif // REPORTPAGE_H
