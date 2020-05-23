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

QT_BEGIN_NAMESPACE

class Ui_InitialPage
{
public:
    QLabel *label;

    void setupUi(QDialog *InitialPage)
    {
        if (InitialPage->objectName().isEmpty())
            InitialPage->setObjectName(QString::fromUtf8("InitialPage"));
        InitialPage->resize(767, 484);
        label = new QLabel(InitialPage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(260, 160, 231, 91));

        retranslateUi(InitialPage);

        QMetaObject::connectSlotsByName(InitialPage);
    } // setupUi

    void retranslateUi(QDialog *InitialPage)
    {
        InitialPage->setWindowTitle(QCoreApplication::translate("InitialPage", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("InitialPage", "hello!", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InitialPage: public Ui_InitialPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INITIALPAGE_H
