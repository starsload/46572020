/********************************************************************************
** Form generated from reading UI file 'initialpage.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INITIALPAGE_H
#define UI_INITIALPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_InitialPage
{
public:
    QLabel *label;
    QPushButton *pushButton;

    void setupUi(QDialog *InitialPage)
    {
        if (InitialPage->objectName().isEmpty())
            InitialPage->setObjectName(QString::fromUtf8("InitialPage"));
        InitialPage->resize(767, 484);
        label = new QLabel(InitialPage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(260, 160, 231, 91));
        pushButton = new QPushButton(InitialPage);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(340, 370, 75, 23));

        retranslateUi(InitialPage);

        QMetaObject::connectSlotsByName(InitialPage);
    } // setupUi

    void retranslateUi(QDialog *InitialPage)
    {
        InitialPage->setWindowTitle(QCoreApplication::translate("InitialPage", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("InitialPage", "\345\210\235\345\247\213\345\214\226\351\241\265\351\235\242", nullptr));
        pushButton->setText(QCoreApplication::translate("InitialPage", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InitialPage: public Ui_InitialPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INITIALPAGE_H
