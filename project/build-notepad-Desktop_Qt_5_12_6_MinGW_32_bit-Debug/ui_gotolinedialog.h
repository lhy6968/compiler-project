/********************************************************************************
** Form generated from reading UI file 'gotolinedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GOTOLINEDIALOG_H
#define UI_GOTOLINEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GoToLineDialog
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *spinBox;
    QPushButton *pushButton;

    void setupUi(QWidget *GoToLineDialog)
    {
        if (GoToLineDialog->objectName().isEmpty())
            GoToLineDialog->setObjectName(QString::fromUtf8("GoToLineDialog"));
        GoToLineDialog->resize(279, 51);
        layoutWidget = new QWidget(GoToLineDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 264, 32));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        spinBox = new QSpinBox(layoutWidget);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMinimum(1);
        spinBox->setMaximum(999999999);

        horizontalLayout->addWidget(spinBox);

        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);


        retranslateUi(GoToLineDialog);

        QMetaObject::connectSlotsByName(GoToLineDialog);
    } // setupUi

    void retranslateUi(QWidget *GoToLineDialog)
    {
        GoToLineDialog->setWindowTitle(QApplication::translate("GoToLineDialog", "Form", nullptr));
        label->setText(QApplication::translate("GoToLineDialog", "Line Number:", nullptr));
        pushButton->setText(QApplication::translate("GoToLineDialog", "Go", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GoToLineDialog: public Ui_GoToLineDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GOTOLINEDIALOG_H
