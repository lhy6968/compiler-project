#ifndef ITYPE_H
#define ITYPE_H

// This header file is to transfer a I-type MIPS command to its binary form.

#include <fstream>
#include <bitset>
#include "objassM.h"
#include "stringProcessing.h"

#include <iostream>

using namespace std;

// to map one I-instruction with its particular opCode.
unordered_map<string, int> opCode_I{
        {"addi", 0x8}, {"addiu", 0x9}, {"andi", 0xc}, {"xori", 0xe}, {"lui", 0xf},
        {"slti", 0xa}, {"sltiu", 0xb}, {"beq", 0x4}, {"bgez", 0x1}, {"bgezal", 0x1},
        {"bgtz", 0x7}, {"blez", 0x6}, {"bltzal", 0x1}, {"bltz", 0x1}, {"bne", 0x5},
        {"teqi", 0x1}, {"tnei", 0x1}, {"ori", 0xd}, {"tgei", 0x1}, {"tgeiu", 0x1},
        {"tlti", 0x1}, {"tltiu", 0x1}, {"lb", 0x20}, {"lbu", 0x24}, {"lh", 0x21},
        {"lhu", 0x25}, {"lw", 0x23}, {"lwl", 0x22}, {"lwr", 0x26}, {"ll", 0x30},
        {"sb", 0x28}, {"sh", 0x29}, {"sw", 0x2b}, {"swl", 0x2a}, {"swr", 0x2e},
        {"sc", 0x38},

};


struct Icommand{
    int opc;
    int rs_rgs;
    int rt_rgs;
    short imm;
};

// for I4(Instr rs, label) & I5(Instr rs, imm) type command, it is mentionable that they have specific values 
//	in the position of "rd" even though they don't have the rd argument.
// therefore, one unordered map is estabilished to map the instruction and the coresponding specific rd-value.
unordered_map<string, int> specifit_rt_I{
    {"bgez", 0x1}, {"bgezal", 0x11}, {"bgtz", 0}, {"blez", 0}, {"bltzal", 0x10},
    {"bltz", 0},
    {"teqi", 0xc}, {"tnei", 0xe},  {"tgei", 0x8}, {"tgeiu", 0x9}, {"tlti", 0xa},
    {"tltiu", 0xb},
};


// to transfer the I-type information array to a 32-bit binary code.
void binary_I(int asb_pc, Icommand I_command, objInfo & objif){
    int instr = (I_command.opc << 26) + (I_command.rs_rgs << 21) + (I_command.rt_rgs << 16) + (int)(unsigned short)I_command.imm;
//    cout <<instr << "a" << endl;
    objif.addToTextSeg(asb_pc, instr);
}

// to transfer I1_type (Instr rt, rs, imm) command to an information array and then to its binary form.
void transfer_I1(int asb_pc, string instr, string line, objInfo & objif){
    int opc = opCode_I[instr];
    int rt_rgs = rgsOrder[slicePara(line)];
    int rs_rgs = rgsOrder[slicePara(line)];
    short imm = stoi(slicePara(line));
    Icommand I_command = {opc, rs_rgs, rt_rgs, imm};
    binary_I(asb_pc,I_command, objif);
}


// to transfer I2_type (Instr rt, imm) command to an information array and then to its binary form.
void transfer_I2(int asb_pc, string instr, string line, objInfo & objif){
    int opc = opCode_I[instr];
    int rt_rgs = rgsOrder[slicePara(line)];
    short imm = stoi(slicePara(line));
    Icommand I_command = {opc, 0, rt_rgs, imm};
    binary_I(asb_pc,I_command, objif);
}

// to transfer I3_type (Instr rs, rt, label) command to an information array and then to its binary form.
void transfer_I3(int asb_pc, string instr, string line, objInfo & objif){
    int opc = opCode_I[instr];
    int rs_rgs = rgsOrder[slicePara(line)];
    int rt_rgs = rgsOrder[slicePara(line)];
    string label_str = slicePara(line);
    int label = objif.locateLabel(asb_pc, opc, label_str, 1);
    int offset = (label - asb_pc) / 4;
    short offset_short = (short int) offset;
    Icommand I_command = {opc, rs_rgs, rt_rgs, offset_short};
    binary_I(asb_pc,I_command, objif);
}


// to transfer I4_type (Instr rs, label) command to an information array and then to its binary form.
void transfer_I4(int asb_pc, string instr, string line, objInfo & objif){
    int opc = opCode_I[instr];
    int rs_rgs = rgsOrder[slicePara(line)];
    int rt_rgs = specifit_rt_I[instr];
    string label_str = slicePara(line);
    int label = objif.locateLabel(asb_pc, opc, label_str, 1);
    int offset = (label - asb_pc) / 4;
    short offset_short = (short int) offset;
    Icommand I_command  = {opc, rs_rgs, rt_rgs, offset_short};
    binary_I(asb_pc,I_command, objif);
}


// to transfer I5_type (Instr rs, imm) command to an information array and then to its binary form.
void transfer_I5(int asb_pc, string instr, string line, objInfo & objif){
    int opc = opCode_I[instr];
    int rs_rgs = rgsOrder[slicePara(line)];
    int rt_rgs = specifit_rt_I[instr];
    short imm = stoi(slicePara(line));
    Icommand I_command  = {opc, rs_rgs, rt_rgs, imm};
    binary_I(asb_pc,I_command, objif);
}


// to transfer I6_type (Instr rt, offset(rs)) command to an information array and then to its binary form.
void transfer_I6(int asb_pc, string instr, string line, objInfo & objif){
    int opc = opCode_I[instr];
    int rt_rgs = rgsOrder[slicePara(line)];
    string token = slicePara(line);
    int lf_ps = token.find("(");
    int rt_ps = token.find(")");
    string offstr = token.substr(0, lf_ps);
    short offset = stoi(offstr);
    int rs_rgs = rgsOrder[token.substr(lf_ps + 1, rt_ps - lf_ps - 1)];
    objif.memOperation(asb_pc, opc, offstr);
    Icommand I_command = {opc, rs_rgs, rt_rgs, offset};
    binary_I(asb_pc,I_command, objif);
}

#endif // ITYPE_H
