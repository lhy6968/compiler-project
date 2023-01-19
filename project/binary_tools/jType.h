#ifndef JTYPE_H
#define JTYPE_H

// This header file is to transfer a J-type MIPS command to its binary form.

#include <fstream>
#include <bitset>
#include "objassM.h"
#include "stringProcessing.h"
#include <iostream>
using namespace std;


// to map one J-instruction with its particular opCode.
unordered_map<string, int> opCode_J{
    {"j", 0x2}, {"jal", 0x3},
};


// to transfer the J-type information array to a 32-bit binary code.
void binary_J(int asb_pc, int J_command[2], objInfo & objif){
    int instr = (J_command[0] << 26) + J_command[1];
    objif.addToTextSeg(asb_pc, instr);
}


// to transfer J-type (Instr target) command to an information array and then to its binary form.
void transfer_J(int asb_pc, string instr, string line, objInfo & objif){
    int opc = opCode_J[instr];
    string label = slicePara(line);
    int target = objif.locateLabel(asb_pc, opc, label, 1);
    int J_command[2] = {opc, target>>2};
    binary_J(asb_pc, J_command, objif);
}

#endif // JTYPE_H
