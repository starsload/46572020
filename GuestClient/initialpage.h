#ifndef INITIALPAGE_H
#define INITIALPAGE_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class InitialPage;
}

class InitialParameters {
public:
	QString address;
	QString port;
	int tempThreshold; //阈值：目标温度与实际温度的差值
	double realTemp; //房间实际温度
	int RoomId; //房间号
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
	void on_btn_yes_clicked();

private:
	Ui::InitialPage *ui;

	bool isNum(QString s);
};

#endif // INITIALPAGE_H
