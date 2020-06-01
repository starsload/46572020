#ifndef INITIALPAGE_H
#define INITIALPAGE_H

#include <QDialog>

namespace Ui {
class InitialPage;
}

class InitialParameters {
public:
	QString address;
	QString port;
	int defaultTargetTemp;
	int maxTargetTemp;
	int minTargetTemp;
	double highFeeRate;
	double middleFeeRate;
	double lowFeeRate;
	int mode; //0是制冷，1是制热
};

class InitialPage : public QDialog
{
	Q_OBJECT

public:
	explicit InitialPage(QWidget *parent = nullptr);
	~InitialPage();

signals:
	void initialFinish(InitialParameters parameters);

private slots:
	void on_ptn_yes_clicked();

private:
	Ui::InitialPage *ui;
};

#endif // INITIALPAGE_H
