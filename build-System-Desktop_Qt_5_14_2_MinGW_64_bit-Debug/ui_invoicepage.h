/********************************************************************************
** Form generated from reading UI file 'invoicepage.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INVOICEPAGE_H
#define UI_INVOICEPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_InvoicePage
{
public:
    QLabel *label;
    QPushButton *CreateDR;
    QTextBrowser *textBrowser;
    QPushButton *Print;
    QLabel *ID;

    void setupUi(QDialog *InvoicePage)
    {
        if (InvoicePage->objectName().isEmpty())
            InvoicePage->setObjectName(QString::fromUtf8("InvoicePage"));
        InvoicePage->resize(374, 547);
        label = new QLabel(InvoicePage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(110, 60, 51, 21));
        CreateDR = new QPushButton(InvoicePage);
        CreateDR->setObjectName(QString::fromUtf8("CreateDR"));
        CreateDR->setGeometry(QRect(130, 480, 75, 23));
        textBrowser = new QTextBrowser(InvoicePage);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(50, 100, 256, 311));
        Print = new QPushButton(InvoicePage);
        Print->setObjectName(QString::fromUtf8("Print"));
        Print->setGeometry(QRect(130, 430, 75, 23));
        ID = new QLabel(InvoicePage);
        ID->setObjectName(QString::fromUtf8("ID"));
        ID->setGeometry(QRect(160, 60, 81, 21));

        retranslateUi(InvoicePage);

        QMetaObject::connectSlotsByName(InvoicePage);
    } // setupUi

    void retranslateUi(QDialog *InvoicePage)
    {
        InvoicePage->setWindowTitle(QCoreApplication::translate("InvoicePage", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("InvoicePage", "\346\210\277\351\227\264\345\217\267\357\274\232", nullptr));
        CreateDR->setText(QCoreApplication::translate("InvoicePage", "\347\224\237\346\210\220\350\257\246\345\215\225", nullptr));
        Print->setText(QCoreApplication::translate("InvoicePage", "\346\211\223\345\215\260\350\264\246\345\215\225", nullptr));
        ID->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class InvoicePage: public Ui_InvoicePage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INVOICEPAGE_H
