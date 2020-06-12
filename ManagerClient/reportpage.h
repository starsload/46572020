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

signals:
	void Print_Report(int RoomId, QString date);

private slots:
	void on_PrintButton_clicked();

private:
	Ui::ReportPage *ui;
};

#endif // REPORTPAGE_H
