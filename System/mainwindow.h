#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <initialpage.h>
#include <queryinvoiceinputdialog.h>
#include <invoicepage.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_pushButton_clicked();
	void queryInputFinish(int);

private:
	Ui::MainWindow *ui;
	InitialPage *initialPage;
	QueryInvoiceInputDialog *queryInvoiceInputDialg;
	InvoicePage *invoicePage;
};
#endif // MAINWINDOW_H
