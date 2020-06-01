#ifndef INVOICEPAGE_H
#define INVOICEPAGE_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class InvoicePage;
}

class InvoicePage : public QDialog
{
	Q_OBJECT

public:
	explicit InvoicePage(QWidget *parent = nullptr, int Room_Id = 0);
	~InvoicePage();

private slots:
	void on_Print_clicked();

	void on_CreateDR_clicked();

private:
	Ui::InvoicePage *ui;
};

#endif // INVOICEPAGE_H
