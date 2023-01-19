#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <QMainWindow>
#include <QString>
#include "register.h"
#include "instmemory.h"
#include "bit.h"
#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <fstream>
#include "LoaderAndSimulator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Debugger; }
QT_END_NAMESPACE

class Debugger
{
public:
    Debugger();
    int getNextPC() const;
    bool next();
    void setInst(const instMemory & instructions);
    Reg * regs;
    instMemory instMem;
    std::map<QString, QString> Memory;
    int size;
    int current_ptr;
    void getreg(Reg * regsrc);
    int init();


    //all global variables
    //create a vector to store all the lines read from the txt file
    std::vector<std::string> txtList;
    //create a vector to store all the machine code translated from each instruction
    std::vector<std::string> machineList;
    //create a vector to store all the lines in data section
    std::vector<std::string> linesList;
    //create a string to store every line of the txt file
    std::string str;
    //create a vector to store relative virtual address of machine code.
    std::vector<std::string> RVA_found;

    //create a one-to-one map to store labels.
    std::map<std::string, int> label_found;

    int int_biggest_RVA;

    //simulate all the registers
    void *hi_void;
    void *lo_void;
    void *registers;

    //simulate PC
    int pc = 0;

    //simulate the memory
    void *main_memory = malloc(6291456);

    //simulate int pointer of the memory
    int *int_memory = (int *)main_memory;

    //simulate char pointer of the memory
    char *char_memory = (char *)main_memory;

    //use int sbrk_number to store the address of sbrk space
    int sbrk_number = 0;

    //use int data_end to store the fake address of the end of static data
    int data_end;

    //use int new_number to store the beginning of the fake address
    int new_number;

    //initializes the value in the zero registers
    int *zero1;
};


#endif // DEBUGGER_H
