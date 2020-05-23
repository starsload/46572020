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
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InitialPage
{
public:
    QSpinBox *spinBox;
    QLabel *label;
    QPushButton *pushButton;
    QLabel *label_2;

    void setupUi(QWidget *InitialPage)
    {
        if (InitialPage->objectName().isEmpty())
            InitialPage->setObjectName(QString::fromUtf8("InitialPage"));
        InitialPage->resize(439, 256);
        spinBox = new QSpinBox(InitialPage);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setGeometry(QRect(230, 110, 51, 21));
        label = new QLabel(InitialPage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(150, 110, 61, 21));
        pushButton = new QPushButton(InitialPage);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(180, 190, 75, 23));
        label_2 = new QLabel(InitialPage);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(160, 30, 101, 21));
        label_2->setAlignment(Qt::AlignCenter);

        retranslateUi(InitialPage);

        QMetaObject::connectSlotsByName(InitialPage);
    } // setupUi

    void retranslateUi(QWidget *InitialPage)
    {
        InitialPage->setWindowTitle(QCoreApplication::translate("InitialPage", "Form", nullptr));
        label->setText(QCoreApplication::translate("InitialPage", "\346\270\251\346\216\247\351\230\210\345\200\274", nullptr));
        pushButton->setText(QCoreApplication::translate("InitialPage", "\347\241\256\345\256\232", nullptr));
        label_2->setText(QCoreApplication::translate("InitialPage", "\345\210\235\345\247\213\345\214\226\350\256\276\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InitialPage: public Ui_InitialPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INITIALPAGE_H
