#include "perferencedialog.h"

PerferenceDialog::PerferenceDialog(Config *config, QWidget *parent) :
    QDialog(parent), config(config)
{
    setupUi(this);
    setWindowIcon(QIcon(tr(":images/notepad.png")));
    setWindowTitle(tr("Preference"));
    //General
    maxRecentFileSpinBox->setValue(config->maxRecentFiles);
    showReadmeCheck->setChecked(config->showReadme);
    showLineNumberCheck->setChecked(config->showLineNumber);

    //Editor
    fontFamilyComboBox->setCurrentFont(QFont(config->fontFamily));
    fontSizeSpinBox->setValue(config->fontSize);
    fontStyleComboBox->setCurrentIndex(
                fontStyleComboBox->findText(config->fontStyle));
    tabIndentsCheck->setChecked(config->tabIndents);
    autoIndentCheck->setChecked(config->autoIndent);
    backUnindentCheck->setChecked(config->backUnindent);
    spaceTabsCheck->setChecked(config->spaceTabs);
    indentSizeSpinBox->setValue(config->indentSize);
    tabSizeSpinBox->setValue(config->tabSize);
    whitespacesCheck->setChecked(config->whitespaces);

    //Search/Replace
    maxHistorySpinBox->setValue(config->maxHistory);
    matchCaseCheck->setVisible(config->matchCase);
    regExpCheck->setChecked(config->regExp);

    connect(applyButton, SIGNAL(clicked()), this, SLOT(apply()));
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
}


void PerferenceDialog::accept()
{
    apply();
    QDialog::accept();
}


void PerferenceDialog::closeEvent(QCloseEvent *e)
{
    e->accept();
}

PerferenceDialog::~PerferenceDialog()
{

}


void PerferenceDialog::apply()
{
    //General
    config->maxRecentFiles = maxRecentFileSpinBox->value();
    config->showReadme = showReadmeCheck->isChecked();
    config->showLineNumber = showLineNumberCheck->isChecked();

    //Editor
    config->fontFamily = fontFamilyComboBox->currentFont().family();
    config->fontSize = fontSizeSpinBox->value();
    config->fontStyle = fontStyleComboBox->currentText();
    config->tabIndents = tabIndentsCheck->isChecked();
    config->autoIndent = autoIndentCheck->isChecked();
    config->backUnindent = backUnindentCheck->isChecked();
    config->spaceTabs = spaceTabsCheck->isChecked();
    config->indentSize = indentSizeSpinBox->value();
    config->tabSize = tabSizeSpinBox->value();
    config->whitespaces = whitespacesCheck->isChecked();

    //Search/Replace
    config->maxHistory = maxHistorySpinBox->value();
    config->matchCase = matchCaseCheck->isChecked();
    config->regExp = regExpCheck->isChecked();

    config->reconfig(Config::Editor);
}
