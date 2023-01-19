#ifndef LOADER_H
#define LOADER_H
#include<string>
using namespace std;


//delete spaces that are not important
void trim2(string &s);

//delete comments in txt files that are not related to translation
string clean_instruction(string str);

//convert decimal digits to binary
string dec_to_binary(int i, int x);

//the functions of all the instructions required as follows:

int binStr_to_decInt(string str);

int new_address(int address,int init_address);

string long_dec_to_binary (long i,int x);

int binReg_to_decInt(string str);

int hexStr_to_dec(string s);

void addi_func (int* rs, int imm,int* rt);

void add_func (int* rs, int* rt,int* rd);

int bne_func(int* rs,int *rt,int pc,int offset);

void sub_func(int* rs,int* rt,int* rd);

void lw_func(int* rs,int* rt,int offset);

void sw_func(int* rt,int* rs,int offset);

void lui_func(int* rt,int imm);

void ori_func(int* rs,int imm,int* rt);

int jal_func(int pc, int* ra,int target);

int beq_func(int* rs,int *rt,int pc,int offset);

int j_func(int pc,int target);

int jr_func(int* rs);

void addu_func (int* rs, int* rt,int* rd);

void addiu_func (int* rs, int imm,int* rt);

void and_func(int* rs,int* rt,int* rd);

void andi_func(int* rs,int imm,int* rt);

void div_func (int* rs,int* rt,int* lo,int* hi);

void divu_func (int* rs,int* rt,int* lo,int* hi);

void mult_func (int* rs,int* rt,int * lo,int* hi);

void multu_func (int* rs,int* rt,int * lo,int* hi);

void nor_func(int* rs,int* rt,int* rd);

void or_func(int* rs,int* rt,int* rd);

void sll_func (int shamt, int* rt,int* rd);

void sllv_func (int* rs, int* rt,int* rd);

void sra_func (int shamt, int* rt,int* rd);

void srav_func (int* rs, int* rt,int* rd);

void srl_func (int shamt, int* rt,int* rd);

void srlv_func (int* rs, int* rt,int* rd);

void subu_func(int* rs,int* rt,int* rd);

void xor_func(int* rs,int* rt,int* rd);

void xori_func(int* rs,int imm,int* rt);

void slt_func (int* rs, int* rt,int* rd);

void sltu_func (int* rs, int* rt,int* rd);

void slti_func(int* rs, int imm,int* rt);

void sltiu_func(int* rs, int imm,int* rt);

int bgez_func(int* rs,int pc,int offset);

int bgezal_func(int* rs,int pc,int offset);

int bgtz_func(int* rs,int pc,int offset);

int blez_func(int* rs,int pc,int offset);

int bltz_func(int* rs,int pc,int offset);

int bltzal_func(int* rs,int pc,int offset);

int jalr_func(int pc,int *rs,int* rd);

void teq_func(int* rs, int* rt,int pc);

void teqi_func(int* rs,int imm,int pc);

void tne_func(int* rs, int* rt,int pc);

void tnei_func(int* rs,int imm,int pc);

void tge_func(int* rs, int* rt,int pc);

void tgeu_func(int* rs, int* rt,int pc);

void tgei_func(int* rs,int imm,int pc);

void tgeiu_func(int* rs,int imm,int pc);

void tlt_func(int* rs, int* rt,int pc);

void tltu_func(int* rs, int* rt,int pc);

void tlti_func(int* rs,int imm,int pc);

void tltiu_func(int* rs,int imm,int pc);

void mfhi_func(int* rd,int* hi);

void mflo_func(int* rd,int* lo);

void mthi_func(int* rs,int* lo);

void mtlo_func(int* rs,int* lo);

int loader();

#endif // LOADER_H
