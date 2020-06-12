#ifndef INVOICEPAGE_H
#define INVOICEPAGE_H

#include <QDialog>
#include <QMessageBox>
#include "Invoice.h"
#include <QString>
#include "DetailRecords.h"

namespace Ui {
class InvoicePage;
}

class InvoicePage : public QDialog
{
	Q_OBJECT

public:
	explicit InvoicePage(QWidget *parent, const Invoice &v);
	~InvoicePage();

signals:
	void PrintInvoice();

	void PrintDR(int);

private slots:
	void on_Print_clicked();

	void on_CreateDR_clicked();

private:
	Ui::InvoicePage *ui;

	Invoice invoice;

	int room_id;
};

#endif // INVOICEPAGE_H
