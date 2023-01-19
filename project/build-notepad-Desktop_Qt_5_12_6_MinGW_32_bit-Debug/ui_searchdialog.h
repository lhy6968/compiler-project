/********************************************************************************
** Form generated from reading UI file 'searchdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCHDIALOG_H
#define UI_SEARCHDIALOG_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SearchDialog
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QComboBox *replaceCombo;
    QLabel *label_1;
    QComboBox *findCombo;
    QHBoxLayout *horizontalLayout_1;
    QToolButton *replacePreviousButton;
    QToolButton *replaceNextButton;
    QPushButton *replaceAllButton;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *findPreviousButton;
    QToolButton *findNextButton;
    QSpacerItem *horizontalSpacer_1;
    QCheckBox *matchCaseCheck;
    QCheckBox *regExpCheck;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *SearchDialog)
    {
        if (SearchDialog->objectName().isEmpty())
            SearchDialog->setObjectName(QString::fromUtf8("SearchDialog"));
        SearchDialog->resize(744, 75);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SearchDialog->sizePolicy().hasHeightForWidth());
        SearchDialog->setSizePolicy(sizePolicy);
        SearchDialog->setMinimumSize(QSize(0, 75));
        SearchDialog->setMaximumSize(QSize(16777215, 75));
        SearchDialog->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        gridLayout_2 = new QGridLayout(SearchDialog);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_2 = new QLabel(SearchDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        replaceCombo = new QComboBox(SearchDialog);
        replaceCombo->setObjectName(QString::fromUtf8("replaceCombo"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(replaceCombo->sizePolicy().hasHeightForWidth());
        replaceCombo->setSizePolicy(sizePolicy2);
        replaceCombo->setMinimumSize(QSize(200, 0));
        replaceCombo->setEditable(true);
        replaceCombo->setMaxCount(100);
        replaceCombo->setInsertPolicy(QComboBox::InsertAtTop);

        gridLayout->addWidget(replaceCombo, 1, 1, 1, 1);

        label_1 = new QLabel(SearchDialog);
        label_1->setObjectName(QString::fromUtf8("label_1"));
        sizePolicy1.setHeightForWidth(label_1->sizePolicy().hasHeightForWidth());
        label_1->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label_1, 0, 0, 1, 1);

        findCombo = new QComboBox(SearchDialog);
        findCombo->setObjectName(QString::fromUtf8("findCombo"));
        sizePolicy2.setHeightForWidth(findCombo->sizePolicy().hasHeightForWidth());
        findCombo->setSizePolicy(sizePolicy2);
        findCombo->setMinimumSize(QSize(200, 0));
        findCombo->setEditable(true);
        findCombo->setMaxCount(100);
        findCombo->setInsertPolicy(QComboBox::InsertAtTop);

        gridLayout->addWidget(findCombo, 0, 1, 1, 1);

        horizontalLayout_1 = new QHBoxLayout();
        horizontalLayout_1->setSpacing(6);
        horizontalLayout_1->setObjectName(QString::fromUtf8("horizontalLayout_1"));
        replacePreviousButton = new QToolButton(SearchDialog);
        replacePreviousButton->setObjectName(QString::fromUtf8("replacePreviousButton"));
        replacePreviousButton->setArrowType(Qt::LeftArrow);

        horizontalLayout_1->addWidget(replacePreviousButton);

        replaceNextButton = new QToolButton(SearchDialog);
        replaceNextButton->setObjectName(QString::fromUtf8("replaceNextButton"));
        replaceNextButton->setArrowType(Qt::RightArrow);

        horizontalLayout_1->addWidget(replaceNextButton);

        replaceAllButton = new QPushButton(SearchDialog);
        replaceAllButton->setObjectName(QString::fromUtf8("replaceAllButton"));

        horizontalLayout_1->addWidget(replaceAllButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_1->addItem(horizontalSpacer_3);


        gridLayout->addLayout(horizontalLayout_1, 1, 2, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        findPreviousButton = new QToolButton(SearchDialog);
        findPreviousButton->setObjectName(QString::fromUtf8("findPreviousButton"));
        findPreviousButton->setArrowType(Qt::LeftArrow);

        horizontalLayout_2->addWidget(findPreviousButton);

        findNextButton = new QToolButton(SearchDialog);
        findNextButton->setObjectName(QString::fromUtf8("findNextButton"));
        findNextButton->setArrowType(Qt::RightArrow);

        horizontalLayout_2->addWidget(findNextButton);

        horizontalSpacer_1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_1);

        matchCaseCheck = new QCheckBox(SearchDialog);
        matchCaseCheck->setObjectName(QString::fromUtf8("matchCaseCheck"));
        matchCaseCheck->setAutoExclusive(false);

        horizontalLayout_2->addWidget(matchCaseCheck);

        regExpCheck = new QCheckBox(SearchDialog);
        regExpCheck->setObjectName(QString::fromUtf8("regExpCheck"));
        regExpCheck->setAutoExclusive(false);

        horizontalLayout_2->addWidget(regExpCheck);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout_2, 0, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

#ifndef QT_NO_SHORTCUT
        label_2->setBuddy(replaceCombo);
        label_1->setBuddy(findCombo);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(findCombo, findNextButton);
        QWidget::setTabOrder(findNextButton, findPreviousButton);
        QWidget::setTabOrder(findPreviousButton, replaceCombo);
        QWidget::setTabOrder(replaceCombo, replaceNextButton);
        QWidget::setTabOrder(replaceNextButton, replacePreviousButton);
        QWidget::setTabOrder(replacePreviousButton, replaceAllButton);
        QWidget::setTabOrder(replaceAllButton, matchCaseCheck);
        QWidget::setTabOrder(matchCaseCheck, regExpCheck);

        retranslateUi(SearchDialog);

        QMetaObject::connectSlotsByName(SearchDialog);
    } // setupUi

    void retranslateUi(QWidget *SearchDialog)
    {
        SearchDialog->setWindowTitle(QString());
        label_2->setText(QApplication::translate("SearchDialog", "&Replace with:", nullptr));
        label_1->setText(QApplication::translate("SearchDialog", "&Find:", nullptr));
        replacePreviousButton->setText(QApplication::translate("SearchDialog", "...", nullptr));
        replaceNextButton->setText(QApplication::translate("SearchDialog", "...", nullptr));
        replaceAllButton->setText(QApplication::translate("SearchDialog", "Replace All", nullptr));
        findPreviousButton->setText(QApplication::translate("SearchDialog", "...", nullptr));
        findNextButton->setText(QApplication::translate("SearchDialog", "...", nullptr));
        matchCaseCheck->setText(QApplication::translate("SearchDialog", "&Case sensitive", nullptr));
        regExpCheck->setText(QApplication::translate("SearchDialog", "Use regular e&xpressions", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SearchDialog: public Ui_SearchDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHDIALOG_H
