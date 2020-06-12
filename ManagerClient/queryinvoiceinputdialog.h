#ifndef QUERYINVOICEINPUTDIALOG_H
#define QUERYINVOICEINPUTDIALOG_H

#include <QDialog>

namespace Ui {
class QueryInvoiceInputDialog;
}

class QueryInvoiceInputDialog : public QDialog
{
	Q_OBJECT

public:
	explicit QueryInvoiceInputDialog(QWidget *parent = nullptr);
	~QueryInvoiceInputDialog();

signals:
	void queryFinish(int Room_Id);

private slots:
	void on_pushButton_2_clicked();

	void on_pushButton_clicked();

private:
	Ui::QueryInvoiceInputDialog *ui;
};

#endif // QUERYINVOICEINPUTDIALOG_H
