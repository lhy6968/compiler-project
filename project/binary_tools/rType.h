#ifndef RTYPE_H
#define RTYPE_H

// This header file is to transfer a R-type MIPS command to its binary form.

#include <fstream>
#include <bitset>
#include "objassM.h"
#include "stringProcessing.h"
#include <iostream>
using namespace std;


// to map one R-instruction with its particular function code.
unordered_map<string, int > funct_R{
    {"add", 0x20}, {"addu", 0x21}, {"and", 0x24}, {"div", 0x1a}, {"divu", 0x1b},
    {"mult", 0x18}, {"multu", 0x19}, {"nor", 0x27}, {"or", 0x25}, {"sll", 0x0},
    {"sllv", 0x4}, {"sra", 0x3}, {"srav", 0x7}, {"srl", 0x2}, {"srlv", 0x6},
    {"jalr", 0x9}, {"jr", 0x8}, {"sub", 0x22}, {"subu", 0x23}, {"xor", 0x26},
    {"slt", 0x2a}, {"sltu", 0x2b}, {"teq", 0x34}, {"tne", 0x36}, {"tge", 0x30},
    {"tgeu", 0x31}, {"tlt", 0x32}, {"tltu", 0x33}, {"mfhi", 0x10}, {"mflo", 0x12},
    {"mthi", 0x11}, {"mtlo", 0x13}, {"syscall", 0xc},
};


// to transfer the R-type information array to a 32-bit binary code.
void binary_R(int asb_pc, int R_command[5], objInfo & objif){
    int instr = (R_command[0] << 21) + (R_command[1] << 16) + (R_command[2] << 11) + (R_command[3] << 6) + R_command[4];
//    cout <<instr <<"c" << endl;
    objif.addToTextSeg(asb_pc, instr);
}


// to transfer R1-type (Instr rd, rs, rt) command to an information array and then to its binary form.
void transfer_R1(int asb_pc, string instr, string line, objInfo & objif){
    int funct_c = funct_R[instr];
    int rd_rgs = rgsOrder[slicePara(line)];
    int rs_rgs = rgsOrder[slicePara(line)];
    int rt_rgs = rgsOrder[slicePara(line)];
    int R_command[5] = {rs_rgs, rt_rgs, rd_rgs, 0, funct_c};
    binary_R(asb_pc, R_command, objif);
}


// to transfer R2-type (Instr rs, rt) command to an information array and then to its binary form.
void transfer_R2(int asb_pc, string instr, string line, objInfo & objif){
    int funct_c = funct_R[instr];
    int rs_rgs = rgsOrder[slicePara(line)];
    int rt_rgs = rgsOrder[slicePara(line)];
    int R_command[5] = {rs_rgs, rt_rgs, 0, 0, funct_c};
    binary_R(asb_pc, R_command, objif);
}


// to transfer R3-type (Instr rd, rt, const) command to an information array and then to its binary form.
void transfer_R3(int asb_pc, string instr, string line, objInfo & objif){
    int funct_c = funct_R[instr];
    int rd_rgs = rgsOrder[slicePara(line)];
    int rt_rgs = rgsOrder[slicePara(line)];
    int shamt = stoi(slicePara(line));
    int R_command[5] = {0, rt_rgs, rd_rgs, shamt, funct_c};
    binary_R(asb_pc, R_command, objif);
}


// to transfer R4-type (Instr rd, rt, rs) command to an information array and then to its binary form.
void transfer_R4(int asb_pc, string instr, string line, objInfo & objif){
    int funct_c = funct_R[instr];
    int rd_rgs = rgsOrder[slicePara(line)];
    int rt_rgs = rgsOrder[slicePara(line)];
    int rs_rgs = rgsOrder[slicePara(line)];
    int R_command[5] = {rs_rgs, rt_rgs, rd_rgs, 0, funct_c};
    binary_R(asb_pc, R_command, objif);
}


// to transfer R5-type (Instr rs, rd) command to an information array and then to its binary form.
void transfer_R5(int asb_pc, string instr, string line, objInfo & objif){
    int funct_c = funct_R[instr];
    int rs_rgs = rgsOrder[slicePara(line)];
    int rd_rgs = rgsOrder[slicePara(line)];
    int R_command[5] = {rs_rgs, 0, rd_rgs, 0, funct_c};
    binary_R(asb_pc, R_command, objif);
}


// to transfer R6-type (Instr rs) command to an information array and then to its binary form.
void transfer_R6(int asb_pc, string instr, string line, objInfo & objif){
    int funct_c = funct_R[instr];
    int rs_rgs = rgsOrder[slicePara(line)];
    int R_command[5] = {rs_rgs, 0, 0, 0, funct_c};
    binary_R(asb_pc, R_command, objif);
}


// to transfer R7-type (Instr rd) command to an information array and then to its binary form.
void transfer_R7(int asb_pc, string instr, string line, objInfo & objif){
    int funct_c = funct_R[instr];
    int rd_rgs = rgsOrder[slicePara(line)];
    int R_command[5] = {0, 0, rd_rgs, 0, funct_c};
    binary_R(asb_pc, R_command, objif);
}


// to transfer R8-type (syscall) command to an information array and then to its binary form.
void transfer_R8(int asb_pc, string instr, string line, objInfo & objif){
    line = line.substr(0);
    int funct_c = funct_R[instr];
    int R_command[5] = {0, 0, 0, 0, funct_c};
    binary_R(asb_pc, R_command, objif);
}

#endif // RTYPE_H
