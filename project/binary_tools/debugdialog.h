#ifndef GOTOLINEDIALOG_H
#define GOTOLINEDIALOG_H
#include<string>
#include <QWidget>
#include <vector>
#include "register.h"
#include <QTextCursor>
#include "debugger.h"
#include "bit.h"

class GoToLineDialog;

namespace Ui
{
class  GoToLineDialog;
}

class GoToLineDialog: public QWidget
{
    Q_OBJECT

public:
    explicit GoToLineDialog(QWidget *parent = 0);
    void setMaxLineNumber(int lineNumber);
    ~GoToLineDialog();

    void changeReg(std::vector<int> regsrc);

    //Debugger
    Reg * regs;
    QTextCursor cursor;
    Debugger * debugger;
    bool finish = 0;
    void debugRunAll();
    void singleStep();
    void changeCursor(int pc);

signals:
    void gotoLine(int lineNumber);  //begugger begin
private slots:
    void gotoLine();    //emit gotoLine

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

public:
    Ui::GoToLineDialog *ui;
    void display();
};


#endif // GOTOLINEDIALOG_H
