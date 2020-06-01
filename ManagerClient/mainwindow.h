#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <initialpage.h>
#include <queryinvoiceinputdialog.h>
#include <invoicepage.h>
#include <reportpage.h>

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
	void queryInputFinish(int);
	void on_ptn_makeInvoice_clicked();

	void on_ptn_makeReport_clicked();

private:
	Ui::MainWindow *ui;
	InitialPage *initialPage;
	QueryInvoiceInputDialog *queryInvoiceInputDialg;
	InvoicePage *invoicePage;
	ReportPage *reportPage;
};
#endif // MAINWINDOW_H
