/********************************************************************************
** Form generated from reading UI file 'perferencedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PERFERENCEDIALOG_H
#define UI_PERFERENCEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PerferenceDialog
{
public:
    QStackedWidget *stackedWidget;
    QWidget *generalPage;
    QWidget *widget;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_3;
    QCheckBox *showReadmeCheck;
    QSpacerItem *horizontalSpacer_7;
    QCheckBox *showLineNumberCheck;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label;
    QSpinBox *maxRecentFileSpinBox;
    QWidget *editorPage;
    QGridLayout *gridLayout_4;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *label_2;
    QFontComboBox *fontFamilyComboBox;
    QLabel *label_3;
    QSpinBox *fontSizeSpinBox;
    QLabel *label_4;
    QComboBox *fontStyleComboBox;
    QGroupBox *groupBox_2;
    QFormLayout *formLayout_2;
    QHBoxLayout *horizontalLayout;
    QCheckBox *tabIndentsCheck;
    QSpacerItem *horizontalSpacer;
    QLabel *label_2_3;
    QSpinBox *indentSizeSpinBox;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *whitespacesCheck;
    QCheckBox *backUnindentCheck;
    QCheckBox *spaceTabsCheck;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *autoIndentCheck;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_2_4;
    QSpinBox *tabSizeSpinBox;
    QWidget *page;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QSpinBox *maxHistorySpinBox;
    QCheckBox *matchCaseCheck;
    QCheckBox *regExpCheck;
    QListWidget *listWidget;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *applyButton;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *acceptButton;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *cancelButton;

    void setupUi(QDialog *PerferenceDialog)
    {
        if (PerferenceDialog->objectName().isEmpty())
            PerferenceDialog->setObjectName(QString::fromUtf8("PerferenceDialog"));
        PerferenceDialog->resize(628, 390);
        stackedWidget = new QStackedWidget(PerferenceDialog);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(136, 9, 460, 332));
        stackedWidget->setMinimumSize(QSize(460, 332));
        stackedWidget->setMaximumSize(QSize(460, 332));
        generalPage = new QWidget();
        generalPage->setObjectName(QString::fromUtf8("generalPage"));
        widget = new QWidget(generalPage);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 32, 261, 96));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_3 = new QSpacerItem(58, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 0, 1, 1);

        showReadmeCheck = new QCheckBox(widget);
        showReadmeCheck->setObjectName(QString::fromUtf8("showReadmeCheck"));

        gridLayout->addWidget(showReadmeCheck, 0, 1, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(58, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_7, 1, 0, 1, 1);

        showLineNumberCheck = new QCheckBox(widget);
        showLineNumberCheck->setObjectName(QString::fromUtf8("showLineNumberCheck"));

        gridLayout->addWidget(showLineNumberCheck, 1, 1, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_4 = new QSpacerItem(58, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);

        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_5->addWidget(label);

        maxRecentFileSpinBox = new QSpinBox(widget);
        maxRecentFileSpinBox->setObjectName(QString::fromUtf8("maxRecentFileSpinBox"));

        horizontalLayout_5->addWidget(maxRecentFileSpinBox);


        gridLayout->addLayout(horizontalLayout_5, 2, 0, 1, 2);

        stackedWidget->addWidget(generalPage);
        editorPage = new QWidget();
        editorPage->setObjectName(QString::fromUtf8("editorPage"));
        gridLayout_4 = new QGridLayout(editorPage);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        groupBox = new QGroupBox(editorPage);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        formLayout = new QFormLayout(groupBox);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_2);

        fontFamilyComboBox = new QFontComboBox(groupBox);
        fontFamilyComboBox->setObjectName(QString::fromUtf8("fontFamilyComboBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, fontFamilyComboBox);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_3);

        fontSizeSpinBox = new QSpinBox(groupBox);
        fontSizeSpinBox->setObjectName(QString::fromUtf8("fontSizeSpinBox"));
        fontSizeSpinBox->setMinimum(8);

        formLayout->setWidget(1, QFormLayout::FieldRole, fontSizeSpinBox);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_4);

        fontStyleComboBox = new QComboBox(groupBox);
        fontStyleComboBox->addItem(QString());
        fontStyleComboBox->addItem(QString());
        fontStyleComboBox->addItem(QString());
        fontStyleComboBox->addItem(QString());
        fontStyleComboBox->setObjectName(QString::fromUtf8("fontStyleComboBox"));

        formLayout->setWidget(2, QFormLayout::FieldRole, fontStyleComboBox);


        gridLayout_4->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(editorPage);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        formLayout_2 = new QFormLayout(groupBox_2);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tabIndentsCheck = new QCheckBox(groupBox_2);
        tabIndentsCheck->setObjectName(QString::fromUtf8("tabIndentsCheck"));

        horizontalLayout->addWidget(tabIndentsCheck);

        horizontalSpacer = new QSpacerItem(58, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label_2_3 = new QLabel(groupBox_2);
        label_2_3->setObjectName(QString::fromUtf8("label_2_3"));

        horizontalLayout->addWidget(label_2_3);

        indentSizeSpinBox = new QSpinBox(groupBox_2);
        indentSizeSpinBox->setObjectName(QString::fromUtf8("indentSizeSpinBox"));
        indentSizeSpinBox->setMaximumSize(QSize(100, 16777215));
        QFont font;
        font.setFamily(QString::fromUtf8("Bitstream Charter"));
        indentSizeSpinBox->setFont(font);
        indentSizeSpinBox->setWrapping(false);
        indentSizeSpinBox->setMinimum(1);
        indentSizeSpinBox->setMaximum(30);

        horizontalLayout->addWidget(indentSizeSpinBox);


        formLayout_2->setLayout(0, QFormLayout::LabelRole, horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        whitespacesCheck = new QCheckBox(groupBox_2);
        whitespacesCheck->setObjectName(QString::fromUtf8("whitespacesCheck"));

        horizontalLayout_3->addWidget(whitespacesCheck);

        backUnindentCheck = new QCheckBox(groupBox_2);
        backUnindentCheck->setObjectName(QString::fromUtf8("backUnindentCheck"));

        horizontalLayout_3->addWidget(backUnindentCheck);


        formLayout_2->setLayout(2, QFormLayout::LabelRole, horizontalLayout_3);

        spaceTabsCheck = new QCheckBox(groupBox_2);
        spaceTabsCheck->setObjectName(QString::fromUtf8("spaceTabsCheck"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, spaceTabsCheck);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        autoIndentCheck = new QCheckBox(groupBox_2);
        autoIndentCheck->setObjectName(QString::fromUtf8("autoIndentCheck"));

        horizontalLayout_2->addWidget(autoIndentCheck);

        horizontalSpacer_2 = new QSpacerItem(58, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        label_2_4 = new QLabel(groupBox_2);
        label_2_4->setObjectName(QString::fromUtf8("label_2_4"));

        horizontalLayout_2->addWidget(label_2_4);

        tabSizeSpinBox = new QSpinBox(groupBox_2);
        tabSizeSpinBox->setObjectName(QString::fromUtf8("tabSizeSpinBox"));
        tabSizeSpinBox->setMaximumSize(QSize(100, 16777215));
        tabSizeSpinBox->setMinimum(1);
        tabSizeSpinBox->setMaximum(30);

        horizontalLayout_2->addWidget(tabSizeSpinBox);


        formLayout_2->setLayout(1, QFormLayout::LabelRole, horizontalLayout_2);


        gridLayout_4->addWidget(groupBox_2, 1, 0, 1, 1);

        stackedWidget->addWidget(editorPage);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        groupBox_3 = new QGroupBox(page);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(20, 20, 292, 132));
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_6->addWidget(label_5);

        maxHistorySpinBox = new QSpinBox(groupBox_3);
        maxHistorySpinBox->setObjectName(QString::fromUtf8("maxHistorySpinBox"));

        horizontalLayout_6->addWidget(maxHistorySpinBox);


        gridLayout_2->addLayout(horizontalLayout_6, 0, 0, 1, 1);

        matchCaseCheck = new QCheckBox(groupBox_3);
        matchCaseCheck->setObjectName(QString::fromUtf8("matchCaseCheck"));

        gridLayout_2->addWidget(matchCaseCheck, 1, 0, 1, 1);

        regExpCheck = new QCheckBox(groupBox_3);
        regExpCheck->setObjectName(QString::fromUtf8("regExpCheck"));

        gridLayout_2->addWidget(regExpCheck, 2, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 0, 0, 1, 1);

        stackedWidget->addWidget(page);
        listWidget = new QListWidget(PerferenceDialog);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(9, 9, 121, 332));
        listWidget->setMinimumSize(QSize(121, 332));
        listWidget->setMaximumSize(QSize(121, 332));
        layoutWidget_2 = new QWidget(PerferenceDialog);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(300, 350, 311, 32));
        horizontalLayout_7 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        applyButton = new QPushButton(layoutWidget_2);
        applyButton->setObjectName(QString::fromUtf8("applyButton"));

        horizontalLayout_7->addWidget(applyButton);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_5);

        acceptButton = new QPushButton(layoutWidget_2);
        acceptButton->setObjectName(QString::fromUtf8("acceptButton"));

        horizontalLayout_7->addWidget(acceptButton);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_6);

        cancelButton = new QPushButton(layoutWidget_2);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout_7->addWidget(cancelButton);

#ifndef QT_NO_SHORTCUT
        label_2_3->setBuddy(indentSizeSpinBox);
        label_2_4->setBuddy(tabSizeSpinBox);
#endif // QT_NO_SHORTCUT

        retranslateUi(PerferenceDialog);
        QObject::connect(listWidget, SIGNAL(currentRowChanged(int)), stackedWidget, SLOT(setCurrentIndex(int)));

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PerferenceDialog);
    } // setupUi

    void retranslateUi(QDialog *PerferenceDialog)
    {
        PerferenceDialog->setWindowTitle(QApplication::translate("PerferenceDialog", "Dialog", nullptr));
        showReadmeCheck->setText(QApplication::translate("PerferenceDialog", "Show Readme At Start", nullptr));
        showLineNumberCheck->setText(QApplication::translate("PerferenceDialog", "Show Line Number", nullptr));
        label->setText(QApplication::translate("PerferenceDialog", "Max Recent Files:", nullptr));
        groupBox->setTitle(QApplication::translate("PerferenceDialog", "Font", nullptr));
        label_2->setText(QApplication::translate("PerferenceDialog", "Family:", nullptr));
        label_3->setText(QApplication::translate("PerferenceDialog", "Size:", nullptr));
        label_4->setText(QApplication::translate("PerferenceDialog", "Style:", nullptr));
        fontStyleComboBox->setItemText(0, QApplication::translate("PerferenceDialog", "Normal", nullptr));
        fontStyleComboBox->setItemText(1, QApplication::translate("PerferenceDialog", "Bold", nullptr));
        fontStyleComboBox->setItemText(2, QApplication::translate("PerferenceDialog", "Italic", nullptr));
        fontStyleComboBox->setItemText(3, QApplication::translate("PerferenceDialog", "Bold Italic", nullptr));

        groupBox_2->setTitle(QApplication::translate("PerferenceDialog", "Tabs and Indentation", nullptr));
#ifndef QT_NO_TOOLTIP
        tabIndentsCheck->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        tabIndentsCheck->setText(QApplication::translate("PerferenceDialog", "Indents via Tabs", nullptr));
        label_2_3->setText(QApplication::translate("PerferenceDialog", "Indent Size:", nullptr));
#ifndef QT_NO_TOOLTIP
        indentSizeSpinBox->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        indentSizeSpinBox->setSuffix(QApplication::translate("PerferenceDialog", " char", nullptr));
#ifndef QT_NO_TOOLTIP
        whitespacesCheck->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        whitespacesCheck->setText(QApplication::translate("PerferenceDialog", "Show Spaces and Tabs", nullptr));
#ifndef QT_NO_TOOLTIP
        backUnindentCheck->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        backUnindentCheck->setText(QApplication::translate("PerferenceDialog", "Backspace Unindentation", nullptr));
#ifndef QT_NO_TOOLTIP
        spaceTabsCheck->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        spaceTabsCheck->setText(QApplication::translate("PerferenceDialog", "Use Spaces instead of Tabs", nullptr));
#ifndef QT_NO_TOOLTIP
        autoIndentCheck->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        autoIndentCheck->setText(QApplication::translate("PerferenceDialog", "Auto-indentation", nullptr));
        label_2_4->setText(QApplication::translate("PerferenceDialog", "Tab Size:", nullptr));
#ifndef QT_NO_TOOLTIP
        tabSizeSpinBox->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        tabSizeSpinBox->setSuffix(QApplication::translate("PerferenceDialog", " char", nullptr));
        groupBox_3->setTitle(QApplication::translate("PerferenceDialog", "Find/Replace Settings", nullptr));
        label_5->setText(QApplication::translate("PerferenceDialog", "Max Find/Replace History:", nullptr));
        matchCaseCheck->setText(QApplication::translate("PerferenceDialog", "Match Case", nullptr));
        regExpCheck->setText(QApplication::translate("PerferenceDialog", "User Regular Expression", nullptr));

        const bool __sortingEnabled = listWidget->isSortingEnabled();
        listWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("PerferenceDialog", "General", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = listWidget->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("PerferenceDialog", "Editor", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = listWidget->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("PerferenceDialog", "Find/Replace", nullptr));
        listWidget->setSortingEnabled(__sortingEnabled);

        applyButton->setText(QApplication::translate("PerferenceDialog", "Apply", nullptr));
        acceptButton->setText(QApplication::translate("PerferenceDialog", "OK", nullptr));
        cancelButton->setText(QApplication::translate("PerferenceDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PerferenceDialog: public Ui_PerferenceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PERFERENCEDIALOG_H
