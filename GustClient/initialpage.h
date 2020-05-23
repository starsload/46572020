#ifndef INITIALPAGE_H
#define INITIALPAGE_H

#include <QDialog>
#include <QMainWindow>

namespace Ui {
class InitialPage;
}

class InitialPage : public QDialog
{
	Q_OBJECT

public:
	explicit InitialPage(QWidget *parent = nullptr);
	~InitialPage();

private:
	Ui::InitialPage *ui;
};

#endif // INITIALPAGE_H
