/********************************************************************************
** Form generated from reading UI file 'guestpage.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUESTPAGE_H
#define UI_GUESTPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_GuestPage
{
public:
    QPushButton *pushButton;

    void setupUi(QDialog *GuestPage)
    {
        if (GuestPage->objectName().isEmpty())
            GuestPage->setObjectName(QString::fromUtf8("GuestPage"));
        GuestPage->resize(740, 494);
        pushButton = new QPushButton(GuestPage);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(310, 320, 91, 31));

        retranslateUi(GuestPage);

        QMetaObject::connectSlotsByName(GuestPage);
    } // setupUi

    void retranslateUi(QDialog *GuestPage)
    {
        GuestPage->setWindowTitle(QCoreApplication::translate("GuestPage", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("GuestPage", "\347\224\237\346\210\220\350\264\246\345\215\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GuestPage: public Ui_GuestPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUESTPAGE_H
