#ifndef ASSEMBLING_H
#define ASSEMBLING_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include "iType.h"
#include "jType.h"
#include "rType.h"
#include "objassM.h"
#include "stringProcessing.h"
#include <math.h>
#include <vector>
using namespace std;

unordered_map<string, int> Directive_tt = {
    {".asciiz", 0}, {".ascii", 1}, {".word", 2}, {".half", 3}, {".byte", 4},
};

// to scan the whole file and record all the labels and the respective address.
void recordSymbol(ifstream & ifile, objInfo & objif){
        int text_pc = 0;
        int data_pc = 0;

        string line;
        int headerFlag = 0;
        bool flag = 1;      // to indicate whether the content belongs to text section. Only when flag equals to 1 the line belongs to text section.
        while (getline(ifile, line)){
            if (headerFlag < 2){    // to extract the header information from the header of the assembly codes.
                int txti = line.find("# text =");
                int datai = line.find("# static =");
                int end = line.find("B");
                if (txti != -1) {
                    string sizestr = line.substr(txti + 8, end - txti - 8);
                    sizestr = clearHeadTailSpace(sizestr);
                    objif.textsize = stoi(sizestr);
                }
                if (datai != -1) {
                    string sizestr = line.substr(datai + 10, end - txti - 10);
                    sizestr = clearHeadTailSpace(sizestr);
                    objif.datasize = stoi(sizestr);
                }
                headerFlag ++;
            }

            clearComment(line);
            int isString = line.find("\"");
            if (((int)line.find(".data") != -1) && (((int)line.find(".data") < isString) || isString == -1)) {
                flag = 0;
                line = line.substr(line.find(".data") + 5);
            }
            if (flag == 0) {
                if (((int)line.find(".text") != -1) && (((int)line.find(".text") < isString) || isString == -1)) {
                    flag = 1;
                    line = line.substr(line.find(".text") + 5);
                }
            }

            if (flag == 0) {
                line = clearHeadTailSpace(line);
                if (line.length() == 0) continue;
                int data_flag = line.find(":");
                if (data_flag != -1){
                    line = line.substr(0, data_flag);
                    string label = clearHeadTailSpace(line);
                    dataTab datapiece;
                    datapiece.addr = data_pc;
                    datapiece.label = label;
                    objif.datatab.push_back(datapiece);
                }
                data_pc += 4;
            } else {
                line = clearHeadTailSpace(line);
                if (line.length() == 0) continue;
                int label_flag = line.find(":");
                if (label_flag != -1) {
                    line = line.substr(0, label_flag);
                    string label = clearHeadTailSpace(line);
//                    label_addr[label] = text_pc;
                    sTable symbelpiece;
                    symbelpiece.addr = text_pc;
                    symbelpiece.label = label;
                    objif.stable.push_back(symbelpiece);
                    text_pc -= 4;
                }
                text_pc += 4;
            }
        }
}



// to transfer one line of command to its binary form based on the format and subtype of the instruction. [switch]
void transfer(int asb_pc, string line, objInfo & objif){
        string instr = instruction(line);
        formatType instr_fmt = format[instr];
        switch (instr_fmt) {
        case R1: transfer_R1(asb_pc, instr, line, objif); break;
        case R2: transfer_R2(asb_pc, instr, line, objif); break;
        case R3: transfer_R3(asb_pc, instr, line, objif); break;
        case R4: transfer_R4(asb_pc, instr, line, objif); break;
        case R5: transfer_R5(asb_pc, instr, line, objif); break;
        case R6: transfer_R6(asb_pc, instr, line, objif); break;
        case R7: transfer_R7(asb_pc, instr, line, objif); break;
        case R8: transfer_R8(asb_pc, instr, line, objif); break;
        case I1: transfer_I1(asb_pc, instr, line, objif); break;
        case I2: transfer_I2(asb_pc, instr, line, objif); break;
        case I3: transfer_I3(asb_pc, instr, line, objif); break;
        case I4: transfer_I4(asb_pc, instr, line, objif); break;
        case I5: transfer_I5(asb_pc, instr, line, objif); break;
        case I6: transfer_I6(asb_pc, instr, line, objif); break;
        case J: transfer_J(asb_pc, instr, line, objif); break;
        }
};


void textAssembling(int & asb_pc, string line, objInfo & objif){
    line = clearHeadTailSpace(line);
    line = clearLabel(line);
    if (!line.empty()) {	// determining whether the line of instruction is valid.
        asb_pc += 4;
        transfer(asb_pc, line, objif);
    }
}

void assembling(ifstream & ifile, objInfo & objif){
    string line;
    int asb_pc = 0;
    int flag = 1;   // "flag == 0" indicates data section; "flag == 1" indicates text section.
    while (getline(ifile, line)){
        clearComment(line);
        line = clearHeadTailSpace(line);
        if (line.empty()) continue;
        int isString = line.find("\"");
        if (((int) line.find(".data") != -1) && (((int)line.find(".data") < isString) || isString == -1)) {
            flag = 0;
            line = line.substr(line.find(".data") + 5);
        }
        if (flag == 0) {
            if (((int)line.find(".text") != -1) && (((int)line.find(".text") < isString) || isString == -1)) {
                flag = 1;
                line = line.substr(line.find(".text") + 5);
            }
        }
        line = clearHeadTailSpace(line);
        if (line.empty()) continue;
        if (flag == 0) objif.dataseg.push_back(line);
        if (flag == 1) textAssembling(asb_pc, line, objif);
    }
}


#endif // ASSEMBLING_H
