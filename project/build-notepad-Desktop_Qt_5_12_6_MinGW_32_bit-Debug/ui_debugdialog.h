/********************************************************************************
** Form generated from reading UI file 'debugdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEBUGDIALOG_H
#define UI_DEBUGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GoToLineDialog
{
public:
    QListWidget *listWidget;
    QListWidget *listWidget_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QWidget *GoToLineDialog)
    {
        if (GoToLineDialog->objectName().isEmpty())
            GoToLineDialog->setObjectName(QString::fromUtf8("GoToLineDialog"));
        GoToLineDialog->resize(494, 886);
        listWidget = new QListWidget(GoToLineDialog);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(10, 40, 181, 841));
        listWidget_2 = new QListWidget(GoToLineDialog);
        listWidget_2->setObjectName(QString::fromUtf8("listWidget_2"));
        listWidget_2->setGeometry(QRect(190, 40, 301, 841));
        pushButton = new QPushButton(GoToLineDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(30, 10, 151, 28));
        pushButton_2 = new QPushButton(GoToLineDialog);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(280, 10, 93, 28));

        retranslateUi(GoToLineDialog);

        QMetaObject::connectSlotsByName(GoToLineDialog);
    } // setupUi

    void retranslateUi(QWidget *GoToLineDialog)
    {
        GoToLineDialog->setWindowTitle(QApplication::translate("GoToLineDialog", "Form", nullptr));
        pushButton->setText(QApplication::translate("GoToLineDialog", "debug_one_step", nullptr));
        pushButton_2->setText(QApplication::translate("GoToLineDialog", "debug_all", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GoToLineDialog: public Ui_GoToLineDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEBUGDIALOG_H
