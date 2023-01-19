#ifndef RGSORDER_H
#define RGSORDER_H

#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <bitset>
using namespace std;

// Divide all the MIPS instructions into R/I/J formats and several subtypes according to the property of the command.
// And map the formats and the instructions one-to-one.
enum formatType {R1, R2, R3, R4, R5, R6, R7, R8, I1, I2, I3, I4, I5, I6, J};

unordered_map<string, formatType> format{
    {"add", R1}, {"addu", R1}, {"and", R1}, {"sub", R1}, {"subu", R1},
    {"xor", R1}, {"slt", R1}, {"sltu", R1}, {"nor", R1}, {"or", R1},          ///R1 >> Instr: rd, rs, rt
    {"div", R2}, {"divu", R2}, {"mult", R2}, {"multu", R2}, {"teq", R2},
    {"tne", R2}, {"tge", R2}, {"tgeu", R2}, {"tlt", R2}, {"tltu", R2},        ///R2 >> Instr: rs, rt
    {"sll", R3}, {"sra", R3}, {"srl", R3},                                    ///R3 >> Instr: rd, rt, const
    {"sllv", R4}, {"srav", R4},  {"srlv", R4},                                ///R4 >> Instr: rd, rt, rs
    {"jalr", R5},                                                             ///R5 >> Instr: rs, rd
    {"jr", R6}, {"mthi", R6}, {"mtlo", R6},                                   ///R6 >> Instr: rs
    {"mfhi", R7}, {"mflo", R7},                                               ///R7 >> Instr: rd
    {"syscall", R8},

    {"addi", I1}, {"addiu", I1}, {"andi", I1}, {"xori", I1}, {"slti", I1},
    {"sltiu", I1}, {"ori", I1},                                               ///I1 >> Instr: rt, rs, imm
    {"lui", I2},                                                              ///I2 >> Instr: rt, imm
    {"beq", I3}, {"bne", I3},                                                 ///I3 >> Instr: rs, rt, label
    {"bgez", I4}, {"bgezal", I4}, {"bgtz", I4}, {"blez", I4}, {"bltzal", I4},
    {"bltz", I4},                                                             ///I4 >> Instr: rs, label
    {"teqi", I5}, {"tnei", I5},  {"tgei", I5}, {"tgeiu", I5}, {"tlti", I5},
    {"tltiu", I5},                                                            ///I5 >> Instr: rs, imm
    {"lb", I6}, {"lbu", I6}, {"lh", I6}, {"lhu", I6}, {"lw", I6},
    {"lwl", I6}, {"lwr", I6}, {"ll", I6}, {"sb", I6}, {"sh", I6},
    {"sw", I6}, {"swl", I6}, {"swr", I6}, {"sc", I6},                         ///I6 >> Instr: rt, offset(rs)

    {"j", J}, {"jal", J},                                                     ///J >> Instr: label
};

// to map one register to its order number.
unordered_map<string, int> rgsOrder{
    {"$zero", 0}, {"$at", 1}, {"$v0", 2}, {"$v1", 3}, {"$a0", 4},
    {"$a1", 5}, {"$a2", 6}, {"$a3", 7}, {"$t0", 8}, {"$t1", 9},
    {"$t2", 10}, {"$t3", 11}, {"$t4", 12}, {"$t5", 13}, {"$t6", 14},
    {"$t7", 15}, {"$s0", 16}, {"$s1", 17}, {"$s2", 18}, {"$s3", 19},
    {"$s4", 20}, {"$s5", 21}, {"$s6", 22}, {"$s7", 23}, {"$t8", 24},
    {"$t9", 25}, {"$k0", 26}, {"$k1", 27}, {"$gp", 28}, {"$sp", 29},
    {"$fp", 30}, {"$ra", 31},
};

unordered_map<int, formatType> reloType{
    {0x2, J}, {0x3, J}, {0x4, I3}, {0x1,I4}, {0x5, I3},
    {0x7, I4}, {0x6, I4},
    {0x20, I6}, {0x24, I6}, {0x21, I6}, {0x25, I6}, {0x23, I6},
    {0x22, I6}, {0x26, I6}, {0x30, I6}, {0x28, I6}, {0x29, I6},
    {0x2b, I6}, {0x2a, I6}, {0x2e, I6}, {0x38, I6},
};

int TEXT_BASE = 0x400000;
int DATA_BASE = 0x500000;
int GLOBLEPOINTER = 0x500000;

struct rInfo{
    int addr;
    int instrType;
    string depend;
};

struct sTable{
    string label;
    int addr;
};

struct textSeg{
    int addr;
    int instr;
};

struct dataTab{
    string label;
    int addr;
};

class objInfo{
public:
    int textsize;
    int datasize;
    vector<rInfo> rinfo;
    vector<textSeg> textseg;
    vector<string> dataseg;
    vector<dataTab> datatab;
    vector<sTable> stable;

    objInfo();

    void addToTextSeg(int asb_pc, int instr);
    int matchLabel(string label);
    int locateLabel(int asb_pc, int opc, string label, int flag);
    void memOperation(int asb_pc, int opc, string data);
};

objInfo::objInfo(){
    textsize = 0;
    datasize = 0;
}

void objInfo::addToTextSeg(int asb_pc, int instr){
    textSeg instrLine;
    instrLine.addr = asb_pc - 4;
    instrLine.instr = instr;
    textseg.push_back(instrLine);
}

int objInfo::matchLabel(string label){
    int addr = -1;
    int i = 0;
    int size = stable.size();
    while (addr == -1 && i < size) {
        if (stable[i].label == label) {
            addr = stable[i].addr;
            break;
        }
        i++;
    }
    return addr;
}

int objInfo::locateLabel(int asb_pc, int opc, string label, int flag){  // flag == 1, record the relocation information; otherwise not.
    int addr = matchLabel(label);
    if (addr == -1 && flag == 1) {
        rInfo rinfopiece;
        rinfopiece.addr = asb_pc;
        rinfopiece.instrType = opc;
        rinfopiece.depend = label;
        rinfo.push_back(rinfopiece);
        addr = 0;
    }
    return addr;
}

void objInfo::memOperation(int asb_pc, int opc, string data){
    rInfo rinfopiece;
    rinfopiece.addr = asb_pc - 4;
    rinfopiece.instrType = opc;
    rinfopiece.depend = data;
    rinfo.push_back(rinfopiece);
}

void headerFMT(ofstream & ofile, int textsize, int datasize){
    ofile.setf(ios::right);
    ofile << setw(10) <<  "Text size" << setw(20) << textsize << " B" << endl;
    ofile << setw(10) <<  "Data size" << setw(20) << datasize << " B" << endl;
};

void relocationInfoFMT(ofstream & ofile, vector<rInfo> & rinfo){
    ofile.setf(ios::right);
    ofile << setw(10) << "Address" << setw(20) << "Instruction Type" << setw(20) << "Dependency" << endl;
    for (int i = 0; i < (int) rinfo.size(); i++){
        ofile << setw(10) << hex << rinfo[i].addr << setw(20) << rinfo[i].instrType << setw(20) << rinfo[i].depend << endl;
    }
};

void symbolTableFMT(ofstream & ofile, vector<sTable> & stable, vector<dataTab> & datatab){
    ofile.setf(ios::right);
    ofile << setw(10) << "Address" << setw(10) << "Label" << endl;
    for (int i = 0; i < (int) stable.size(); i++){
        ofile << setw(10) << hex << stable[i].addr << setw(10) << stable[i].label << endl;
    }
    for (int i = 0; i < (int) datatab.size(); i++) {
        ofile << setw(10) << hex << datatab[i].addr << setw(10) << datatab[i].label << endl;
    }
};


void testSegFMT(ofstream & ofile, vector<textSeg> & testseg){
    for (int i = 0; i < (int) testseg.size(); i++){
        ofile << hex << testseg[i].addr << ":" << bitset<32>(testseg[i].instr) << endl;
    }
}

void dataSegFMT(ofstream & ofile, vector<string> & dataseg){
    for (int i = 0; i < (int) dataseg.size(); i++) {
        ofile << dataseg[i] << endl;
    }
}

void formatOutput(ofstream & ofile, objInfo & objif){
    ofile << "Header" << endl;
    headerFMT(ofile, objif.textsize, objif.datasize);
    ofile << endl;
    ofile << "Text segment" << endl;
    testSegFMT(ofile, objif.textseg);
    ofile << endl;
    ofile << "Data segment" << endl;
    dataSegFMT(ofile, objif.dataseg);
    ofile << endl;
    ofile << "Relocation infomation" << endl;
    relocationInfoFMT(ofile, objif.rinfo);
    ofile << endl;
    ofile << "Symbol table" << endl;
    symbolTableFMT(ofile, objif.stable, objif.datatab);
    ofile << endl;
}


#endif // RGSORDER_H
