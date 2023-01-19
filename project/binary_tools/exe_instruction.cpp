#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <math.h>
#include "LoaderAndSimulator.h"
using namespace std;

vector<int> get_registers(void *registers, void *hi_void, void *lo_void, int pc){
    vector<int> data_in_reg;
    int *register_int =(int *)registers;
    for(int i=0; i<32; i++){
        data_in_reg.push_back(*(register_int+i));
    }
    int *hi_void_int =(int *)hi_void;
    int *lo_void_int =(int *)lo_void;
    data_in_reg.push_back(*hi_void_int);
    data_in_reg.push_back(*lo_void_int);
    data_in_reg.push_back(pc);
    return data_in_reg;
}

vector<int> exe_one_instruct(int pc, int new_number, int *int_memory, void *registers, void *hi_void,
                      void *lo_void, char * char_memory, int sbrk_number, int data_end, int *zero1, int line_num, bool &finish){
    //execute one instruction
    pc = new_number + line_num * 4;
    int offset = (pc - new_number)/4;
    string instruction = dec_to_binary(*(int_memory+offset),32);
    //cout << *(int_memory + offset) << endl;
    string instruction1 = instruction.substr(0,6);
    string instruction2 = instruction.substr(21,5);
    string instruction3 = instruction.substr(26,6);
    string instruction4 = instruction.substr(11,5);
    //add instruction
    if ((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="100000")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string rdStr = instruction.substr(16,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int rd = binReg_to_decInt(rdStr);
            int *register1 = (int *)registers;
            add_func ((register1+rs), (register1+rt),(register1+rd));
            if ((*(register1+rd)>0)&&(*(register1+rs)<0)&&(*(register1+rt)<0)){
                    printf("exception occurs at %d",pc);
                    exit(1);
            }
            else if((*(register1+rd)<0)&&(*(register1+rs)>0)&&(*(register1+rt)>0)){
                    printf("exception occurs at %d",pc);
                    exit(1);
            }
            pc = pc + 4;
    }
    //addu instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="100001")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string rdStr = instruction.substr(16,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int rd = binReg_to_decInt(rdStr);
            int *register1 = (int *)registers;
            addu_func ((register1+rs), (register1+rt),(register1+rd));
            pc = pc + 4;
    }
    //addi instruction
    else if(instruction1=="001000"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string immStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int imm = binStr_to_decInt(immStr);
            int *register1 = (int *)registers;
            addi_func ((register1+rs), imm,(register1+rt));
            if ((*(register1+rt)>0)&&(*(register1+rs)<0)&&(imm<0)){
                    printf("exception occurs at %d",pc);
                    exit(1);
            }
            else if((*(register1+rt)<0)&&(*(register1+rs)>0)&&(imm>0)){
                    printf("exception occurs at %d",pc);
                    exit(1);
            }
            pc = pc + 4;
    }
    //addiu instruction
    else if(instruction1=="001001"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string immStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int imm = binStr_to_decInt(immStr);
            int *register1 = (int *)registers;
            addiu_func ((register1+rs), imm,(register1+rt));
            pc = pc + 4;
    }
    //and instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="100100")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string rdStr = instruction.substr(16,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int rd = binReg_to_decInt(rdStr);
            int *register1 = (int *)registers;
            and_func ((register1+rs), (register1+rt),(register1+rd));
            pc = pc + 4;
    }
    //andi instruction
    else if(instruction1=="001100"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string immStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int imm = binStr_to_decInt(immStr);
            int *register1 = (int *)registers;
            andi_func ((register1+rs), imm,(register1+rt));
            pc = pc + 4;
    }
    //div instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="011010")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int *register1 = (int *)registers;
            int *hi = (int *)hi_void;
            int *lo = (int *)lo_void;
            if (*(register1+rt)==0){
                    printf("exception occurs at %d",pc);
                    exit(1);
            }
            div_func ((register1+rs),(register1+rt),lo,hi);
            pc = pc + 4;
    }
    //divu instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="011011")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int *register1 = (int *)registers;
            int *hi = (int *)hi_void;
            int *lo = (int *)lo_void;
            divu_func ((register1+rs),(register1+rt),lo,hi);
            pc = pc + 4;
    }
    //mult instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="011000")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int *register1 = (int *)registers;
            int *hi = (int *)hi_void;
            int *lo = (int *)lo_void;
            mult_func ((register1+rs),(register1+rt),lo,hi);
            pc = pc + 4;
    }
    //multu instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="011001")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int *register1 = (int *)registers;
            int *hi = (int *)hi_void;
            int *lo = (int *)lo_void;
            multu_func ((register1+rs),(register1+rt),lo,hi);
            pc = pc + 4;
    }
    //nor instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="100111")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string rdStr = instruction.substr(16,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int rd = binReg_to_decInt(rdStr);
            int *register1 = (int *)registers;
            nor_func ((register1+rs), (register1+rt),(register1+rd));
            pc = pc + 4;
    }
    //or instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="100101")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string rdStr = instruction.substr(16,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int rd = binReg_to_decInt(rdStr);
            int *register1 = (int *)registers;
            or_func ((register1+rs), (register1+rt),(register1+rd));
            pc = pc + 4;
    }
    //ori instruction
    else if(instruction1=="001101"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string immStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int imm = binStr_to_decInt(immStr);
            int *register1 = (int *)registers;
            ori_func((register1+rs),imm,(register1+rt));
            pc = pc + 4;
    }
    //sll instruction
    else if ((instruction1=="000000")&&(instruction3=="000000")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string rdStr = instruction.substr(16,5);
            string shamtStr = instruction.substr(21,5);
            int rt = binReg_to_decInt(rtStr);
            int rd = binReg_to_decInt(rdStr);
            int shamt = binReg_to_decInt(shamtStr);
            int *register1 = (int *)registers;
            sll_func (shamt, (register1+rt),(register1+rd));
            pc = pc + 4;
    }
    //sllv instruction
    else if ((instruction1=="000000")&&(instruction3=="000100")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string rdStr = instruction.substr(16,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int rd = binReg_to_decInt(rdStr);
            int *register1 = (int *)registers;
            sllv_func ((register1+rs), (register1+rt),(register1+rd));
            pc = pc + 4;
    }
    //sra instruction
    else if ((instruction1=="000000")&&(instruction3=="000011")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string rdStr = instruction.substr(16,5);
            string shamtStr = instruction.substr(21,5);
            int rt = binReg_to_decInt(rtStr);
            int rd = binReg_to_decInt(rdStr);
            int shamt = binReg_to_decInt(shamtStr);
            int *register1 = (int *)registers;
            sra_func (shamt, (register1+rt),(register1+rd));
            pc = pc + 4;
    }
    //srav instruction
    else if ((instruction1=="000000")&&(instruction3=="000111")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string rdStr = instruction.substr(16,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int rd = binReg_to_decInt(rdStr);
            int *register1 = (int *)registers;
            srav_func ((register1+rs), (register1+rt),(register1+rd));
            pc = pc + 4;
    }
    //srl instruction
    else if ((instruction1=="000000")&&(instruction3=="000010")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string rdStr = instruction.substr(16,5);
            string shamtStr = instruction.substr(21,5);
            int rt = binReg_to_decInt(rtStr);
            int rd = binReg_to_decInt(rdStr);
            int shamt = binReg_to_decInt(shamtStr);
            int *register1 = (int *)registers;
            srl_func (shamt, (register1+rt),(register1+rd));
            pc = pc + 4;
    }
    //srlv instruction
    else if ((instruction1=="000000")&&(instruction3=="000110")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string rdStr = instruction.substr(16,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int rd = binReg_to_decInt(rdStr);
            int *register1 = (int *)registers;
            srlv_func ((register1+rs), (register1+rt),(register1+rd));
            pc = pc + 4;
    }
    //sub instruction
    else if ((instruction1=="000000")&&(instruction3=="100010")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string rdStr = instruction.substr(16,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int rd = binReg_to_decInt(rdStr);
            int *register1 = (int *)registers;
            sub_func((register1+rs), (register1+rt),(register1+rd));
            pc = pc + 4;
    }
    //subu instruction
    else if ((instruction1=="000000")&&(instruction3=="100011")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string rdStr = instruction.substr(16,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int rd = binReg_to_decInt(rdStr);
            int *register1 = (int *)registers;
            subu_func((register1+rs), (register1+rt),(register1+rd));
            pc = pc + 4;
    }
    //xor instruction
    else if ((instruction1=="000000")&&(instruction3=="100110")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string rdStr = instruction.substr(16,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int rd = binReg_to_decInt(rdStr);
            int *register1 = (int *)registers;
            xor_func ((register1+rs), (register1+rt),(register1+rd));
            pc = pc + 4;
    }
    //xori instruction
    else if (instruction1=="001110"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string immStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int imm = binStr_to_decInt(immStr);
            int *register1 = (int *)registers;
            xori_func((register1+rs),imm,(register1+rt));
            pc = pc + 4;
    }
    //lui instruction
    else if (instruction1=="001111"){
            string rtStr = instruction.substr(11,5);
            string immStr = instruction.substr(16,16);
            int rt = binReg_to_decInt(rtStr);
            int imm = binStr_to_decInt(immStr);
            int *register1 = (int *)registers;
            lui_func((register1+rt),imm);
            pc = pc + 4;
    }
    //slt instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="101010")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string rdStr = instruction.substr(16,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int rd = binReg_to_decInt(rdStr);
            int *register1 = (int *)registers;
            slt_func ((register1+rs), (register1+rt),(register1+rd));
            pc = pc + 4;
    }
    //sltu instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="101011")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string rdStr = instruction.substr(16,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int rd = binReg_to_decInt(rdStr);
            int *register1 = (int *)registers;
            sltu_func ((register1+rs), (register1+rt),(register1+rd));
            pc = pc + 4;
    }
    //slti instruction
    else if (instruction1=="001010"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string immStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int imm = binStr_to_decInt(immStr);
            int *register1 = (int *)registers;
            slti_func((register1+rs),imm,(register1+rt));
            pc = pc + 4;
    }
    //sltiu instruction
    else if (instruction1=="001011"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string immStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int imm = binStr_to_decInt(immStr);
            int *register1 = (int *)registers;
            sltiu_func((register1+rs),imm,(register1+rt));
            pc = pc + 4;
    }
    //beq instruction
    else if (instruction1=="000100"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            pc = beq_func((register1+rs),(register1+rt),pc,offset);
    }
    //bgez instruction
    else if ((instruction1=="000001")&&(instruction4=="00001")){
            string rsStr = instruction.substr(6,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            pc = bgez_func((register1+rs),pc,offset);
    }
    //bgezal instruction
    else if ((instruction1=="000001")&&(instruction4=="10001")){
            string rsStr = instruction.substr(6,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            *(register1+31) = pc;
            pc = bgezal_func((register1+rs),pc,offset);
    }
    //bgtz instruction
    else if ((instruction1=="000111")&&(instruction4=="00000")){
            string rsStr = instruction.substr(6,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            pc = bgtz_func((register1+rs),pc,offset);
    }
    //blez instruction
    else if ((instruction1=="000110")&&(instruction4=="00000")){
            string rsStr = instruction.substr(6,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            pc = blez_func((register1+rs),pc,offset);
    }
    //bltzal instruction
    else if ((instruction1=="000001")&&(instruction4=="10000")){
            string rsStr = instruction.substr(6,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            *(register1+31) = pc;
            pc = bltzal_func((register1+rs),pc,offset);
    }
    //bltz instruction
    else if ((instruction1=="000001")&&(instruction4=="00000")){
            string rsStr = instruction.substr(6,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            pc = bltz_func((register1+rs),pc,offset);
    }
    //bne instruction
    else if (instruction1=="000101"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            pc = bne_func((register1+rs),(register1+rt),pc,offset);
    }
    //j instruction
    else if (instruction1=="000010"){
            string targetStr = instruction.substr(6,26);
            int target = binStr_to_decInt(targetStr);
            pc =  j_func(pc,target);
    }
    //jal instruction
    else if (instruction1=="000011"){
            string targetStr = instruction.substr(6,26);
            int target = binStr_to_decInt(targetStr);
            int *register1 = (int *)registers;
            pc = jal_func(pc, (register1+31),target);
    }
    //jalr instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="001001")){
            string rsStr = instruction.substr(6,5);
            string rdStr = instruction.substr(16,5);
            int rs = binReg_to_decInt(rsStr);
            int rd = binReg_to_decInt(rdStr);
            int *register1 = (int *)registers;
            pc = jalr_func(pc, (register1+rs),(register1+rd));
    }
    //jr instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="001000")){
            string rsStr = instruction.substr(6,5);
            int rs = binReg_to_decInt(rsStr);
            int *register1 = (int *)registers;
            pc = jr_func((register1+rs));
    }
    //teq instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="110100")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int *register1 = (int *)registers;
            teq_func((register1+rs),(register1+rt),pc);
            pc = pc + 4;
    }
    //teqi instruction
    else if ((instruction1=="000001")&&(instruction4=="01100")){
            string rsStr = instruction.substr(6,5);
            string immStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int imm = binStr_to_decInt(immStr);
            int *register1 = (int *)registers;
            teqi_func((register1+rs),imm,pc);
            pc = pc + 4;
    }
    //tne instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="110110")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int *register1 = (int *)registers;
            tne_func((register1+rs),(register1+rt),pc);
            pc = pc + 4;
    }
    //tnei instruction
    else if ((instruction1=="000001")&&(instruction4=="01110")){
            string rsStr = instruction.substr(6,5);
            string immStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int imm = binStr_to_decInt(immStr);
            int *register1 = (int *)registers;
            tnei_func((register1+rs),imm,pc);
            pc = pc + 4;
    }
    //tge instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="110000")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int *register1 = (int *)registers;
            tge_func((register1+rs),(register1+rt),pc);
            pc = pc + 4;
    }
    //tgeu instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="110001")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int *register1 = (int *)registers;
            tgeu_func((register1+rs),(register1+rt),pc);
            pc = pc + 4;
    }
    //tgei instruction
    else if ((instruction1=="000001")&&(instruction4=="01000")){
            string rsStr = instruction.substr(6,5);
            string immStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int imm = binStr_to_decInt(immStr);
            int *register1 = (int *)registers;
            tgei_func((register1+rs),imm,pc);
            pc = pc + 4;
    }
    //tgeiu instruction
    else if ((instruction1=="000001")&&(instruction4=="01001")){
            string rsStr = instruction.substr(6,5);
            string immStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int imm = binStr_to_decInt(immStr);
            int *register1 = (int *)registers;
            tgeiu_func((register1+rs),imm,pc);
            pc = pc + 4;
    }
    //tlt instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="110010")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int *register1 = (int *)registers;
            tlt_func((register1+rs),(register1+rt),pc);
            pc = pc + 4;
    }
    //tltu instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="110011")){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int *register1 = (int *)registers;
            tltu_func((register1+rs),(register1+rt),pc);
            pc = pc + 4;
    }
    //tlti instruction
    else if ((instruction1=="000001")&&(instruction4=="01010")){
            string rsStr = instruction.substr(6,5);
            string immStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int imm = binStr_to_decInt(immStr);
            int *register1 = (int *)registers;
            tlti_func((register1+rs),imm,pc);
            pc = pc + 4;
    }
    //tltiu instruction
    else if ((instruction1=="000001")&&(instruction4=="01011")){
            string rsStr = instruction.substr(6,5);
            string immStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int imm = binStr_to_decInt(immStr);
            int *register1 = (int *)registers;
            tltiu_func((register1+rs),imm,pc);
            pc = pc + 4;
    }
    //lb instruction
    else if (instruction1=="100000"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            int address = *(register1+rs) + offset;
            char *lb = (char *)(char_memory + (address-4194304));
            if (isdigit(*lb)){
                    stringstream st1;
                    st1 << *lb;
                    string a;
                    a = st1.str();
                    stringstream st2;
                    st2 << a;
                    int lb_int;
                    st2 >> lb_int;
                    *(register1+rt) = lb_int;
            }
            else{
                    int lb_int = *lb;
                    *(register1+rt) = lb_int;
            }
            pc = pc + 4;
    }
    //lbu instruction
    else if (instruction1=="100100"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            int address = *(register1+rs) + offset;
            char *lb = (char *)(char_memory + (address-4194304));
            int lb_int = *lb;
            *(register1+rt) = lb_int;
            pc = pc + 4;
    }
    //lh instruction
    else if (instruction1=="100001"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            int address = *(register1+rs) + offset;
            short *lh = (short *)(char_memory + (address-4194304));
            int lh_int = *lh;
            *(register1+rt) = lh_int;
            pc = pc + 4;
    }
    //lhu instruction
    else if (instruction1=="100101"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            int address = *(register1+rs) + offset;
            short *lh = (short *)(char_memory + (address-4194304));
            int lh_int = *lh;
            *(register1+rt) = lh_int;
            pc = pc + 4;
    }
    //lw instruction
    else if (instruction1=="100011"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            int address = *(register1+rs) + offset;
            *(register1+rt) = *(int_memory + (address-4194304)/4);
            pc = pc + 4;
    }
    //lwl instruction
    else if (instruction1=="100010"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            int address = *(register1+rs) + offset;
            int lwl = *(int_memory + (address-4194304)/4);
            string lwlStr = dec_to_binary (lwl,32);
            string part1 = lwlStr.substr(0,16);
            int rt1 = *(register1+rt);
            string rt1Str = dec_to_binary (rt1,32);
            string part2 = rt1Str.substr(0,16);
            string all = part2 + part1;
            int result1 = binStr_to_decInt(all);
            *(register1+rt) = result1;
            pc = pc + 4;
    }
    //lwr instruction
    else if (instruction1=="100110"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            int address = *(register1+rs) + offset;
            int lwr = *(int_memory + (address-4194304)/4);
            string lwrStr = dec_to_binary (lwr,32);
            string part1 = lwrStr.substr(16,16);
            int rt1 = *(register1+rt);
            string rt1Str = dec_to_binary (rt1,32);
            string part2 = rt1Str.substr(0,16);
            string all = part2 + part1;
            int result1 = binStr_to_decInt(all);
            *(register1+rt) = result1;
            pc = pc + 4;
    }
    //ll instruction
    else if (instruction1=="110000"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            int address = *(register1+rs) + offset;
            *(register1+rt) = *(int_memory + (address-4194304)/4);
            pc = pc + 4;
    }
    //sb instruction
    else if (instruction1=="101000"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            int address = *(register1+rs) + offset;
            int sb_int = *(register1+rt);
            string sb3 = dec_to_binary (sb_int,32);
            string sb1 = sb3.substr(24,8);
            int sb1_int = binStr_to_decInt(sb1);
            char *sb2 = (char *)(char_memory + (address-4194304));
            *sb2 = sb1_int;
            pc = pc + 4;
    }
    //sh instruction
    else if (instruction1=="101001"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            int address = *(register1+rs) + offset;
            int sh_int = *(register1+rt);
            string sh3 = dec_to_binary (sh_int,32);
            string sh1 = sh3.substr(24,16);
            int sh1_int = binStr_to_decInt(sh1);
            short *sh2 = (short *)(char_memory + (address-4194304));
            *sh2 = sh1_int;
            pc = pc + 4;
    }
    //sw instruction
    else if (instruction1=="101011"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            int address = *(register1+rs) + offset;
            *(int_memory + (address-4194304)/4) = *(register1+rt);
            pc = pc + 4;
    }
    //swl instruction
    else if (instruction1=="101010"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            int address = *(register1+rs) + offset;
            int swl = *(int_memory + (address-4194304)/4);
            string swlStr = dec_to_binary (swl,32);
            string part1 = swlStr.substr(0,16);
            int rt1 = *(register1+rt);
            string rt1Str = dec_to_binary (rt1,32);
            string part2 = rt1Str.substr(0,16);
            string all = part1 + part2;
            int result1 = binStr_to_decInt(all);
            *(int_memory + (address-4194304)/4) = result1;
            pc = pc + 4;
    }
    //swr instruction
    else if (instruction1=="101110"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            int address = *(register1+rs) + offset;
            int swl = *(int_memory + (address-4194304)/4);
            string swlStr = dec_to_binary (swl,32);
            string part1 = swlStr.substr(0,16);
            int rt1 = *(register1+rt);
            string rt1Str = dec_to_binary (rt1,32);
            string part2 = rt1Str.substr(16,16);
            string all = part1 + part2;
            int result1 = binStr_to_decInt(all);
            *(int_memory + (address-4194304)/4) = result1;
            pc = pc + 4;
    }
    //sc instruction
    else if (instruction1=="111000"){
            string rsStr = instruction.substr(6,5);
            string rtStr = instruction.substr(11,5);
            string offsetStr = instruction.substr(16,16);
            int rs = binReg_to_decInt(rsStr);
            int rt = binReg_to_decInt(rtStr);
            int offset = binStr_to_decInt(offsetStr);
            int *register1 = (int *)registers;
            int address = *(register1+rs) + offset;
            *(int_memory + (address-4194304)/4) = *(register1+rt);
            pc = pc + 4;
    }
    //mfhi instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="010000")){
            string rdStr = instruction.substr(16,5);
            int rd = binReg_to_decInt(rdStr);
            int *register1 = (int *)registers;
            int *hi = (int *)hi_void;
            mfhi_func((register1+rd),hi);
            pc = pc + 4;
    }
    //mflo instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="010010")){
            string rdStr = instruction.substr(16,5);
            int rd = binReg_to_decInt(rdStr);
            int *register1 = (int *)registers;
            int *lo = (int *)lo_void;
            mflo_func((register1+rd),lo);
            pc = pc + 4;
    }
    //mthi instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="010001")){
            string rsStr = instruction.substr(6,5);
            int rs = binReg_to_decInt(rsStr);
            int *register1 = (int *)registers;
            int *hi = (int *)hi_void;
            mthi_func((register1+rs),hi);
            pc = pc + 4;
    }
    //mtlo instruction
    else if((instruction1=="000000")&&(instruction2=="00000")&&(instruction3=="010011")){
            string rsStr = instruction.substr(6,5);
            int rs = binReg_to_decInt(rsStr);
            int *register1 = (int *)registers;
            int *lo = (int *)lo_void;
            mtlo_func((register1+rs),lo);
            pc = pc + 4;
    }
    //syscall instruction
    else if(instruction=="00000000000000000000000000001100"){
            FILE *fp = NULL;
            int *v0 = (int *)(zero1 + 2);
            if (*v0 == 5){
                    int number1;
                    cin >> number1;
                    *v0 = number1;
            }
            else if (*v0 == 1){
                    //cout<< *(zero1 + 4)<<endl;
            }
            else if (*v0 == 10){
                    finish = 1;
                    //exit(0);
            }
            else if (*v0 == 12){
                    char chara;
                    cin >> chara;
                    int chara_num = chara;
                    *v0 = chara_num;
            }
            else if (*v0 == 11){
                    //int *a0 = (int *)(v0+2);
                    //char as = *a0;
                    //cout<<as<<endl;
            }
            else if (*v0 == 9){
                    *v0 = data_end + sbrk_number;
                    sbrk_number = sbrk_number + *(v0+2);
            }
            else if (*v0 == 8){
                    int a0 = *(v0+2);
                    char *read = (char *)(int_memory+(a0-4194304)/4);
                    string str;
                    cin >> str;
                    for(int i = 0;i<int(str.length());i=i+1){
                            *read = str[i];
                            read = read + 1;
                    }
            }
            else if (*v0 == 4){
                    char *print_string = (char *)(int_memory+(*(v0+2)-4194304)/4);
                    string str;
                    while (*print_string != '\0'){
                            str = str + *print_string;
                            print_string = print_string + 1;
                    }
                    //cout<<str<<endl;
            }
            else if (*v0 == 13){
                    char *print_string = (char *)(int_memory+(*(v0+2)-4194304)/4);
                    string str;
                    while (*print_string != '\0'){
                            str = str + *print_string;
                            print_string = print_string + 1;
                    }
            //	str = str.substr(1);
                    char *p = (char*)str.c_str();
                    //r
                    if (*(v0+3)==0){
                            fp = fopen(p,"r");
                    }
                    //w
                    else if(*(v0+3)==1){
                            fp = fopen(p,"w");
                    }
                    //rw
                    else if(*(v0+3)==2){
                            fp = fopen(p,"r+");
                    }
                    int fd;
                    fd = fileno(fp);
                    *(v0+2) = fd;
            }
            else if (*v0 == 14){
                    int fd = *(v0+2);
                    fp = fdopen(fd,"r");
                    *(v0+2) = *(v0+4);
                    char buff[255];
                    fgets(buff,*(v0+4),(FILE*)fp);
                    int a1 = *(v0+3);
                    char *read = (char *)(int_memory+(a1-4194304)/4);
                    string str;
                    str = buff;
                    for(int i = 0;i<int(str.length());i=i+1){
                            *read = str[i];
                            read = read + 1;
                    }
            }
            else if(*v0 == 15){
                    int fd = *(v0+2);
                    fp = fdopen(fd,"w");
                    *(v0+2) = *(v0+4);
                    char *print_string = (char *)(int_memory+(*(v0+3)-4194304)/4);
                    string str;
                    for (int i = 0;i<*(v0+4);i=i+1){
                            str = str + *print_string;
                            print_string = print_string + 1;
                    }
                    char *p = (char*)str.c_str();
                    fputs(p,fp);
                    }
            else if(*v0 == 16){
                    int fd = *(v0+2);
                    fp = fdopen(fd,"r");
                    fclose(fp);
            }
            else if (*v0 == 17){
                    //cout<<*(v0+2)<<endl;
                    //exit(0);
            }
            pc = pc + 4;
    }
    return get_registers(registers, hi_void, lo_void, pc);
}


