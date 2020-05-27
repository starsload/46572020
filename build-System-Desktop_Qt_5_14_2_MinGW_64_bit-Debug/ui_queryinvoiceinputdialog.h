/********************************************************************************
** Form generated from reading UI file 'queryinvoiceinputdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUERYINVOICEINPUTDIALOG_H
#define UI_QUERYINVOICEINPUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QueryInvoiceInputDialog
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;

    void setupUi(QDialog *QueryInvoiceInputDialog)
    {
        if (QueryInvoiceInputDialog->objectName().isEmpty())
            QueryInvoiceInputDialog->setObjectName(QString::fromUtf8("QueryInvoiceInputDialog"));
        QueryInvoiceInputDialog->resize(360, 274);
        pushButton = new QPushButton(QueryInvoiceInputDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(90, 200, 75, 23));
        pushButton_2 = new QPushButton(QueryInvoiceInputDialog);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(200, 200, 75, 23));
        widget = new QWidget(QueryInvoiceInputDialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(50, 120, 251, 22));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);


        retranslateUi(QueryInvoiceInputDialog);

        QMetaObject::connectSlotsByName(QueryInvoiceInputDialog);
    } // setupUi

    void retranslateUi(QDialog *QueryInvoiceInputDialog)
    {
        QueryInvoiceInputDialog->setWindowTitle(QCoreApplication::translate("QueryInvoiceInputDialog", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("QueryInvoiceInputDialog", "\347\241\256\345\256\232", nullptr));
        pushButton_2->setText(QCoreApplication::translate("QueryInvoiceInputDialog", "\345\217\226\346\266\210", nullptr));
        label->setText(QCoreApplication::translate("QueryInvoiceInputDialog", "Room_Id:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QueryInvoiceInputDialog: public Ui_QueryInvoiceInputDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUERYINVOICEINPUTDIALOG_H
