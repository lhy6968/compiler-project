#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QWidget>
#include "ui_searchdialog.h"
#include "config.h"

class SearchDialog: public QWidget, private Ui::SearchDialog
{
    Q_OBJECT

public:
    SearchDialog(Config *, QWidget * = 0);
    ~SearchDialog();

signals:
    void search(QString, bool, bool, bool);
    void replace(QString, QString, bool, bool, bool);
    void replaceAll(QString, QString, bool, bool);

private slots:
    void search();
    void replace();

private:
    void update(QComboBox *);

    Config *config;
};

#endif // SEARCHDIALOG_H
