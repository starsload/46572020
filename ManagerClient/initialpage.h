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
	int defaultTargetTemp; //默认目标温度
	int maxTargetTemp; //最高目标温度
	int minTargetTemp; //最低目标温度
	double highFeeRate; //高风速费率
	double middleFeeRate; //中风速费率
	double lowFeeRate; //低风速费率
	int mode; //工作模式：0是制冷，1是制热
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
