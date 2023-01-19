#include "debugdialog.h"
#include "ui_debugdialog.h"
#include<iostream>
#include "debugger.h"
#include "mainwindow.h"
#include <qthread.h>
#include <QString>

using namespace std;


GoToLineDialog::GoToLineDialog(QWidget *parent) :
    QWidget(parent), ui(new Ui:: GoToLineDialog)
{
    regs = new Reg();
    debugger = new Debugger();
    debugger->getreg(regs);

    ui->setupUi(this);
    ui ->listWidget->addItem(new QListWidgetItem(QString("identity")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("pc")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$zero")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$at")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$v0")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$v1")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$a0")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$a1")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$a2")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$a3")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$t0")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$t1")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$t2")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$t3")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$t4")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$t5")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$t6")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$t7")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$s0")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$s1")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$s2")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$s3")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$s4")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$s5")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$s6")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$s7")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$t8")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$t9")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$k0")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$k1")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$gp")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$sp")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$fp")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$ra")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$HI")));
    ui ->listWidget->addItem(new QListWidgetItem(QString("$LO")));
    setWindowIcon(QIcon(tr(":images/notepad.png")));
    setWindowTitle(tr("Goto Line"));
}

GoToLineDialog::~GoToLineDialog()
{
    delete ui;
}


void GoToLineDialog::on_pushButton_clicked()
{
    ui ->listWidget_2->clear();
    singleStep();
}


void GoToLineDialog::display(){
    QListWidgetItem *p1 = new QListWidgetItem(QListWidgetItem(QString("value")));
    QListWidgetItem *p2 = new QListWidgetItem(QString::number(debugger->regs->PC->value));
    QListWidgetItem *p3 = new QListWidgetItem(QString::number(debugger->regs->registers[0]->value));
    QListWidgetItem *p4 = new QListWidgetItem(QString::number(debugger->regs->registers[1]->value));
    QListWidgetItem *p5 = new QListWidgetItem(QString::number(debugger->regs->registers[2]->value));
    QListWidgetItem *p6 = new QListWidgetItem(QString::number(debugger->regs->registers[3]->value));
    QListWidgetItem *p7 = new QListWidgetItem(QString::number(debugger->regs->registers[4]->value));
    QListWidgetItem *p8 = new QListWidgetItem(QString::number(debugger->regs->registers[5]->value));
    QListWidgetItem *p9 = new QListWidgetItem(QString::number(debugger->regs->registers[6]->value));
    QListWidgetItem *p10 = new QListWidgetItem(QString::number(debugger->regs->registers[7]->value));
    QListWidgetItem *p11 = new QListWidgetItem(QString::number(debugger->regs->registers[8]->value));
    QListWidgetItem *p12 = new QListWidgetItem(QString::number(debugger->regs->registers[9]->value));
    QListWidgetItem *p13 = new QListWidgetItem(QString::number(debugger->regs->registers[10]->value));
    QListWidgetItem *p14 = new QListWidgetItem(QString::number(debugger->regs->registers[11]->value));
    QListWidgetItem *p15 = new QListWidgetItem(QString::number(debugger->regs->registers[12]->value));
    QListWidgetItem *p16 = new QListWidgetItem(QString::number(debugger->regs->registers[13]->value));
    QListWidgetItem *p17 = new QListWidgetItem(QString::number(debugger->regs->registers[14]->value));
    QListWidgetItem *p18 = new QListWidgetItem(QString::number(debugger->regs->registers[15]->value));
    QListWidgetItem *p19 = new QListWidgetItem(QString::number(debugger->regs->registers[16]->value));
    QListWidgetItem *p20 = new QListWidgetItem(QString::number(debugger->regs->registers[17]->value));
    QListWidgetItem *p21 = new QListWidgetItem(QString::number(debugger->regs->registers[18]->value));
    QListWidgetItem *p22 = new QListWidgetItem(QString::number(debugger->regs->registers[19]->value));
    QListWidgetItem *p23 = new QListWidgetItem(QString::number(debugger->regs->registers[20]->value));
    QListWidgetItem *p24 = new QListWidgetItem(QString::number(debugger->regs->registers[21]->value));
    QListWidgetItem *p25 = new QListWidgetItem(QString::number(debugger->regs->registers[22]->value));
    QListWidgetItem *p26 = new QListWidgetItem(QString::number(debugger->regs->registers[23]->value));
    QListWidgetItem *p27 = new QListWidgetItem(QString::number(debugger->regs->registers[24]->value));
    QListWidgetItem *p28 = new QListWidgetItem(QString::number(debugger->regs->registers[25]->value));
    QListWidgetItem *p29 = new QListWidgetItem(QString::number(debugger->regs->registers[26]->value));
    QListWidgetItem *p30 = new QListWidgetItem(QString::number(debugger->regs->registers[27]->value));
    QListWidgetItem *p31 = new QListWidgetItem(QString::number(debugger->regs->registers[28]->value));
    QListWidgetItem *p32 = new QListWidgetItem(QString::number(debugger->regs->registers[29]->value));
    QListWidgetItem *p33 = new QListWidgetItem(QString::number(debugger->regs->registers[30]->value));
    QListWidgetItem *p34 = new QListWidgetItem(QString::number(debugger->regs->registers[31]->value));
    QListWidgetItem *p35 = new QListWidgetItem(QString::number(debugger->regs->HI->value));
    QListWidgetItem *p36 = new QListWidgetItem(QString::number(debugger->regs->LO->value));

    ui ->listWidget_2->addItem(p1);
    ui ->listWidget_2->addItem(p2);
    ui ->listWidget_2->addItem(p3);
    ui ->listWidget_2->addItem(p4);
    ui ->listWidget_2->addItem(p5);
    ui ->listWidget_2->addItem(p6);
    ui ->listWidget_2->addItem(p7);
    ui ->listWidget_2->addItem(p8);
    ui ->listWidget_2->addItem(p9);
    ui ->listWidget_2->addItem(p10);
    ui ->listWidget_2->addItem(p11);
    ui ->listWidget_2->addItem(p12);
    ui ->listWidget_2->addItem(p13);
    ui ->listWidget_2->addItem(p14);
    ui ->listWidget_2->addItem(p15);
    ui ->listWidget_2->addItem(p16);
    ui ->listWidget_2->addItem(p17);
    ui ->listWidget_2->addItem(p18);
    ui ->listWidget_2->addItem(p19);
    ui ->listWidget_2->addItem(p20);
    ui ->listWidget_2->addItem(p21);
    ui ->listWidget_2->addItem(p22);
    ui ->listWidget_2->addItem(p23);
    ui ->listWidget_2->addItem(p24);
    ui ->listWidget_2->addItem(p25);
    ui ->listWidget_2->addItem(p26);
    ui ->listWidget_2->addItem(p27);
    ui ->listWidget_2->addItem(p28);
    ui ->listWidget_2->addItem(p29);
    ui ->listWidget_2->addItem(p30);
    ui ->listWidget_2->addItem(p31);
    ui ->listWidget_2->addItem(p32);
    ui ->listWidget_2->addItem(p33);
    ui ->listWidget_2->addItem(p34);
    ui ->listWidget_2->addItem(p35);
    ui ->listWidget_2->addItem(p36);
}


void GoToLineDialog::singleStep() {
    cout << "current PC: " << dec << debugger->regs->PC->value << endl;
    if (!finish) {
        vector<int> regster_value = exe_one_instruct(debugger->pc, debugger->new_number, debugger->int_memory, debugger->registers, debugger->hi_void,
                          debugger->lo_void, debugger->char_memory, debugger->sbrk_number, debugger->data_end, debugger->zero1, (debugger->regs->PC->value - 0x400000) / 4, finish);
        changeReg(regster_value);
        display();
    } else std::cout << "-> Finish debugging successfully!" << endl;
}

void GoToLineDialog::on_pushButton_2_clicked()
{
    ui ->listWidget_2->clear();
    display();
    while (!finish) {
       ui ->listWidget_2->clear();
       singleStep();
       QThread::msleep(10);
    }
    finish = 0;
    regs->fresh();
    debugger->regs->fresh();
    debugger->init();
    cout << "-------------------Run done-------------------" << endl;
}


void GoToLineDialog::changeReg(vector<int> regsrc){
    for (int i = 0; i<32; i++){
        debugger->regs->registers[i]->value = regsrc[i];
    }
    debugger->regs->HI->value = regsrc[32];
    debugger->regs->LO->value = regsrc[33];
    debugger->regs->PC->value = regsrc[34];

}


void GoToLineDialog::setMaxLineNumber(int lineNumber)
{
    lineNumber++;
}


void GoToLineDialog::gotoLine()
{
}

