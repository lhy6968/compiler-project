#include "searchdialog.h"

SearchDialog::SearchDialog(Config *config, QWidget *parent) :
    QWidget(parent), config(config)
{
    setupUi(this);
    setWindowIcon(QIcon(tr(":images/notepad.png")));
    setWindowTitle(tr("Search & Replace"));
    findCombo->setMaxCount(config->maxHistory);
    findCombo->addItems(config->findHistory);
    findCombo->setCurrentIndex(-1);

    replaceCombo->setMaxCount(config->maxHistory);
    replaceCombo->addItems(config->replaceHistory);
    replaceCombo->setCurrentIndex(-1);

    matchCaseCheck->setChecked(config->matchCase);
    regExpCheck->setChecked(config->regExp);

    connect(findNextButton, SIGNAL(clicked()), SLOT(search()));
    connect(findPreviousButton, SIGNAL(clicked()), SLOT(search()));
    connect(replaceNextButton, SIGNAL(clicked()), SLOT(replace()));
    connect(replacePreviousButton, SIGNAL(clicked()), SLOT(replace()));
    connect(replaceAllButton, SIGNAL(clicked()), SLOT(replace()));
}

SearchDialog::~SearchDialog()
{
    config->matchCase = matchCaseCheck->isChecked();
    config->regExp = regExpCheck->isChecked();

    config->findHistory.clear();

    for (int i = 0; i < findCombo->count(); i++)
        config->findHistory << findCombo->itemText(i);

    config->replaceHistory.clear();

    for (int i = 0; i < replaceCombo->count(); i++)
        config->replaceHistory << replaceCombo->itemText(i);
}


void SearchDialog::search()
{
    update (findCombo);

    bool backward = (sender() == findPreviousButton) ? true : false;

    emit search(findCombo->currentText(), backward, matchCaseCheck->isChecked(),
                regExpCheck->isChecked());
}


void SearchDialog::replace()
{
    update (findCombo);
    update (replaceCombo);

    if (sender() == replaceAllButton)
    {
        emit replaceAll(findCombo->currentText(), replaceCombo->currentText(),
                        matchCaseCheck->isChecked(), regExpCheck->isChecked());
    }
    else
    {
        bool backward = (sender() == replacePreviousButton) ? true : false;

        emit replace(findCombo->currentText(), replaceCombo->currentText(),
                     backward, matchCaseCheck->isChecked(),
                     regExpCheck->isChecked());
    }
}

// update the history of search/replace
void SearchDialog::update(QComboBox *combo)
{
    int index = combo->findText(combo->currentText());

    if (index == -1)
        combo->insertItem(0, combo->currentText());
    else
        combo->setCurrentIndex(index);
}
