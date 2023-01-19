#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <iostream>
#include<malloc.h>
#include<bitset>
#include<math.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

using namespace std;

//function declaration
int hexStr_to_dec(string s);

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


int loader(){
    //all global variables
    //create a vector to store all the lines read from the txt file
    vector<string> txtList;
    //create a vector to store all the machine code translated from each instruction
    vector<string> machineList;
    //create a vector to store all the lines in data section
    vector<string> linesList;
    //create a string to store every line of the txt file
    string str;
    //create a one-to-one map to store relative virtual address of machine code.
    map<string, int> RVA_found;
    //create a one-to-one map to store labels.
    map<string, int> label_found;


    //simulate all the registers
    void *hi_void = malloc(4);
    void *lo_void = malloc(4);
    void *registers = malloc(128);

    //simulate PC
    int pc = 0;

    //simulate the memory
    void *main_memory = malloc(6291456);

    //simulate int pointer of the memory
    int *int_memory = (int *)main_memory;
        //cout << int_memory << endl;

    //simulate char pointer of the memory
    char *char_memory = (char *)main_memory;

    ifstream in;
    string f_path = "objlinker.txt";
    //cout << "Please enter the input file name:";
    //cin >> f_path;
    in.open(f_path);

    if (!in)
    {
        cerr << "open " << f_path << "fail" << endl;
        return -1;
    }

    //load all the lines from the txt file
    while (!in.eof()){
        getline(in,str);
        if (str.npos == str.find_first_not_of(" ")){//skip empty lines
            continue;
        }
        str = clean_instruction(str);//delete comment
        trim2(str); //delete some spaces that are not important
        txtList.push_back(str);
    }

    //close the txt input file
    in.close();

    int line_num = 0;//be used to count the line number of machine code
    int label_num = 0;//be used to count which line the label belongs
    int section;
    for (int it=0;it< int(txtList.size());it++){
        if (txtList[it] == "Header"){
            section = 0;
        }
        else if(txtList[it].substr(0,12) == "Text segment"){
            section = 1;
        }
        else if(txtList[it].substr(0,12) == "Data segment"){
            section = 2;
        }
        else if(txtList[it].substr(0,21) == "Relocation infomation"){
            section = 3;

        }
        else if(txtList[it].substr(0,12) == "Symbol table"){
            section = 4;
        }


        if (section == 0 && txtList[it] != "Header"){
            if (txtList[it].substr(0,9) == "Text size"){
                string::size_type pos_int1,pos_B;
                int int_text_size;
                pos_int1 = txtList[it].find_first_not_of(" ",9);
                pos_B = txtList[it].find("B");
                stringstream text_size;
                text_size << txtList[it].substr(pos_int1,pos_B-pos_int1-1);
                text_size >> int_text_size;
                *int_memory = int_text_size;

                //cout << txtList[it] << endl;
                //cout << *int_memory << endl;
            }
            else if (txtList[it].substr(0,9) == "Data size"){
                string::size_type pos_int2,pos2_B;
                int int_data_size;
                pos_int2 = txtList[it].find_first_not_of(" ",9);
                pos2_B = txtList[it].find("B");
                stringstream data_size;
                data_size << txtList[it].substr(pos_int2,pos2_B-pos_int2);
                data_size >> int_data_size;
                *(int_memory+1) = int_data_size;
            }
            else{
                cout << "Error ocurs in header." << endl;
            }
        }
        else if(section == 1 && txtList[it].substr(0,12) != "Text segment"){
            string::size_type p1, p2;
            p2 = txtList[it].find(":");
            p1 = 0;

            if (p2 != txtList[it].npos)
            {
                RVA_found[string(txtList[it].substr(p1,p2))] = line_num; //find and store label
                machineList.push_back (txtList[it].substr(p2+1));
                line_num = line_num +1;
            }
            else{
                machineList.push_back (txtList[it]);
                line_num = line_num +1;
            }
        }
        else if(section == 2 && txtList[it].substr(0,12) != "Data segment"){
            string::size_type pos1, pos2;
            pos2 = txtList[it].find(":");
            pos1 = 0;

            if (pos2 != txtList[it].npos)
            {
                label_found[string(txtList[it].substr(pos1,pos2))] = label_num; //find and store label
                linesList.push_back (txtList[it].substr(pos2+1));
                label_num = label_num +1;
            }
            else{
                machineList.push_back (txtList[it]);
                label_num = label_num +1;
            }

        }
        else if(section == 3 && txtList[it].substr(0,21) != "Relocation infomation"){
            continue;
        }
        else if(section == 4 && txtList[it].substr(0,12) != "Symbol table"){
            continue;
        }

    }


    //use int order to record the offset
    int order = 0;
    for (int i = 0;i<int(machineList.size());i=i+1){
        //cout << machineList[i] << endl;
        int n = binStr_to_decInt(machineList[i]);
        *(int_memory+order) = n;
        order = order + 1;
        };

    //simulate char pointer of the second part:data part
    char *b = (char *)(int_memory+262144);
    //use int order2 to recode the offset
    int order2 = 0;

    //store the .data into data section
    for (int i = 0;i < int(linesList.size());i=i+1 ){
        if (int(linesList[i].find(".asciiz"))!= -1){
            string newStr = linesList[i].substr(linesList[i].find("\"")+1);
            int location = newStr.find("\"");
            string asciizStr = linesList[i].substr(linesList[i].find("\"")+1,location);
            asciizStr = asciizStr + '\0';
            int remainder = asciizStr.length() % 4;
            int left;
            if (remainder == 0){
                left = 0;
            }
            else{
                left = 4 - remainder;
            }
            for (int j = 0;j< int(asciizStr.length());j = j+1){
                *(b+order2) = asciizStr[j];
                order2 = order2 + 1;
            }
            order2 = order2 + left;
        }
        else if (int(linesList[i].find(".word"))!=-1){
            vector<int>wordList;
            string subWordStr = linesList[i].substr(linesList[i].find(".word")+5);
            while (int(subWordStr.find(","))!=-1){
                int word;
                string wordStr = subWordStr.substr(0,subWordStr.find(","));
                stringstream s1;
                s1 << wordStr;
                s1 >> word;
                wordList.push_back(word);
                subWordStr = subWordStr.substr(subWordStr.find(",")+1);
            }
            int lastWord;
            stringstream s2;
            s2 << subWordStr;
            s2 >> lastWord;
            wordList.push_back(lastWord);
            int *c = (int *)(b+order2);
            for (int i = 0;i<int(wordList.size());i=i+1){
                *(c+i) = wordList[i];
                order2 = order2 + 4;
            }
        }
        else if (int(linesList[i].find(".half"))!=-1){
            vector<int>wordList;
            string subWordStr = linesList[i].substr(linesList[i].find(".half")+5);
            while (int(subWordStr.find(","))!=-1){
                int word;
                string wordStr = subWordStr.substr(0,subWordStr.find(","));
                stringstream s1;
                s1 << wordStr;
                s1 >> word;
                wordList.push_back(word);
                subWordStr = subWordStr.substr(subWordStr.find(",")+1);
            }
            int lastWord;
            stringstream s2;
            s2 << subWordStr;
            s2 >> lastWord;
            wordList.push_back(lastWord);
            short *c = (short *)(b+order2);
            for (int i = 0;i<int(wordList.size());i=i+1){
                *(c+i) = wordList[i];
                order2 = order2 + 2;
            }
        }
        else if (int(linesList[i].find(".ascii"))!=-1){
            string newStr = linesList[i].substr(linesList[i].find("\"")+1);
            int location = newStr.find("\"");
            string asciizStr = linesList[i].substr(linesList[i].find("\"")+1,location);
            int remainder = asciizStr.length() % 4;
            int left;
            if (remainder == 0){
                left = 0;
            }
            else{
                left = 4 - remainder;
            }
            for (int j = 0;j< int(asciizStr.length());j = j+1){
                *(b+order2) = asciizStr[j];
                order2 = order2 + 1;
            }
            order2 = order2 + left;
        }
        else if (int(linesList[i].find(".byte"))!=-1){
            vector<char>byteList;
            string subWordStr = linesList[i].substr(linesList[i].find(".byte")+5);
            char byteStr;
            while (int(subWordStr.find(","))!=-1){
                for (int i = 0;i<int(subWordStr.length());i=i+1){
                if (subWordStr[i]!=' '){
                    byteStr = subWordStr[i];
                    byteList.push_back(byteStr);
                    break;
                }
            }
                subWordStr = subWordStr.substr(subWordStr.find(",")+1);
            }
            char lastByte;
            for (int i = 0;i<int(subWordStr.length());i=i+1){
                if (subWordStr[i]!=' '){
                    lastByte = subWordStr[i];
                    byteList.push_back(lastByte);
                    break;
                }
            }
            char *c = (char *)(b+order2);
            for (int i = 0;i<int(byteList.size());i=i+1){
                *(c+i) = byteList[i];
                order2 = order2 + 1;
            }
        }
    };

    //simulate a pointer to record the end of static data
    char *static_data_end =  (char *)(b+order2);

    //translate the address to int
    uintptr_t mm = (uintptr_t)static_data_end;
    uintptr_t m = (uintptr_t)main_memory;

    //simulate a int pointer for the registers
    int *register1 = (int *)registers;
    //initializes the value in the registers
    *register1 = 0;
    *(register1+2) = 0;
    *(register1+3) = 0;
    *(register1+4) = 0;
    *(register1+5) = 0;
    *(register1+6) = 0;
    *(register1+7) = 0;
    *(register1+8) = 0;
    *(register1+9) = 0;
    *(register1+10) = 0;
    *(register1+11) = 0;
    *(register1+12) = 0;
    *(register1+13) = 0;
    *(register1+14) = 0;
    *(register1+15) = 0;
    *(register1+16) = 0;
    *(register1+17) = 0;
    *(register1+18) = 0;
    *(register1+19) = 0;
    *(register1+20) = 0;
    *(register1+21) = 0;
    *(register1+22) = 0;
    *(register1+23) = 0;

    //use int sbrk_number to store the address of sbrk space
    int sbrk_number = 0;

    //use int data_end to store the fake address of the end of static data
    int data_end = new_address(mm,m);

    //use int new_number to store the beginning of the fake address
    int new_number = new_address(m,m);

    //initializes the value in the special registers
    *(register1+29) = new_number + 6291456;
    *(register1+30) = new_number + 6291456;
    *(register1+28) = new_number + 1048576;

    //initializes the value of PC
    pc = new_number;

    //initializes the value in the zero registers
    int *zero1 = (int *)registers;
    *zero1 = 0;


    //execute all the instructions
    while (abs((pc-new_number)/4)< int(machineList.size())){
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
                            cout<< dec << *(zero1 + 4)<<endl;
                    }
                    else if (*v0 == 10){
                            break;
                    }
                    else if (*v0 == 12){
                            char chara;
                            cin >> chara;
                            int chara_num = chara;
                            *v0 = chara_num;
                    }
                    else if (*v0 == 11){
                            int *a0 = (int *)(v0+2);
                            char as = *a0;
                            cout<<as<<endl;
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
                            cout<<str<<endl;
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
                            cout<<*(v0+2)<<endl;
                            exit(0);
                    }
                    pc = pc + 4;
            }
    }
    free(hi_void);
    free(lo_void);
    free(registers);
    free(main_memory);

    return 0;
}

void trim2(string &s)
{
    if( !s.empty() )
    {
        s.erase(0,s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
    }

}

//clean comments and others which do not related to the translation
string clean_instruction(string str){
        string::size_type idx;
        string comment = "#";
        idx=str.find(comment);
        string newStr;
        //exist comment
        if(idx != string::npos ){
        str = str.substr(0,str.find(comment)-1);
    };
    //delete the space and \t at the beginning of each line
    while((str[0] == '\t') or (str[0] == ' ')){
        str = str.substr(1);
        }
        //delete the \n at the end of each line
        string::size_type idx2;
        string comment2 = "\n";
        idx2=str.find(comment2);
        string newStr2;
        //exist comment
        if(idx2 != string::npos ){
        str = str.substr(0,str.find(comment)-1);
    };
    return str;
}

//converts a binary string to a decimal integer
int binStr_to_decInt(string str){
    int number = 0;
    int bit = 0;
    if (str[0]=='1'){
        for (int i = str.length()-1;i>=1;i=i-1){
        if (str[i]=='0'){
            str[i] = '1';
        }
        else{
            str[i] = '0';
            break;
        }
        }
        for (int i = 1;i<int(str.length());i=i+1){
            if (str[i]=='0'){
                str[i] ='1';
            }
            else{
                str[i] = '0';
            }
        }
        for (int i = str.length()-1;i>0;i=i-1){
        if (str[i]=='1'){
            number = number + pow(2,bit);
            bit = bit + 1;
        }
        else{
            bit = bit + 1;
        }
    }
    number = -number;
    }

    else if (str[0]=='0'){
        for (int i = str.length()-1;i>=0;i=i-1){
        if (str[i]=='1'){
            number = number + pow(2,bit);
            bit = bit + 1;
        }
        else{
            bit = bit + 1;
        }
    }
    }
    return number;
}

//translate the 64bit real address to 32bit fake address
int new_address(int address,int init_address){
    int newAddress = address - init_address + 4194304;
    return newAddress;
}

//base to binary
string dec_to_binary (int i,int x)
{
    if (i>=0){
        string binary_number;
        string newStr = "";
        while (i!=0){
        int subNum = i%2;
        i = i/2;
        stringstream ss;
        ss << subNum;
        string subStr;
        ss >> subStr;
        newStr = subStr + newStr;
    }
        binary_number = "00000000000000000000000000000000" + newStr;
        binary_number = binary_number.substr(binary_number.length()-x,binary_number.length()-1);
        return binary_number;
        }
    else{
        i = -i;
        string binary_number;
        string newStr = "";
        while (i!=0){
        int subNum = i%2;
        i = i/2;
        stringstream ss;
        ss << subNum;
        string subStr;
        ss >> subStr;
        newStr = subStr + newStr;
    };

        binary_number = "00000000000000000000000000000000" + newStr;
        binary_number = binary_number.substr(binary_number.length()-x,binary_number.length()-1);
        binary_number = "1" + binary_number.substr(1);
        for (int i = 1;i < int(binary_number.length());i=i+1){

            if (binary_number[i]=='0'){
                binary_number = binary_number.substr(0,i) + "1" +binary_number.substr(i+1);
            }
            else{
                binary_number = binary_number.substr(0,i) + "0" +binary_number.substr(i+1);
            }
        }
        for (int i = binary_number.length() - 1;i>=0;i = i - 1){
            if (binary_number[i]=='0'){
                binary_number = binary_number.substr(0,i) + "1" +binary_number.substr(i+1);
                break;
            }
            else{
                binary_number = binary_number.substr(0,i) + "0" +binary_number.substr(i+1);
            }
        }
        return binary_number;
    }
}

string long_dec_to_binary (long i,int x)
{
    if (i>=0){
        string binary_number;
        string newStr = "";
        while (i!=0){
        int subNum = i%2;
        i = i/2;
        stringstream ss;
        ss << subNum;
        string subStr;
        ss >> subStr;
        newStr = subStr + newStr;
    }
        binary_number = "0000000000000000000000000000000000000000000000000000000000000000" + newStr;
        binary_number = binary_number.substr(binary_number.length()-x,binary_number.length()-1);
        return binary_number;
        }
    else{
        i = -i;
        string binary_number;
        string newStr = "";
        while (i!=0){
        int subNum = i%2;
        i = i/2;
        stringstream ss;
        ss << subNum;
        string subStr;
        ss >> subStr;
        newStr = subStr + newStr;
    };

        binary_number = "00000000000000000000000000000000" + newStr;
        binary_number = binary_number.substr(binary_number.length()-x,binary_number.length()-1);
        binary_number = "1" + binary_number.substr(1);
        for (int i = 1;i < int(binary_number.length());i=i+1){

            if (binary_number[i]=='0'){
                binary_number = binary_number.substr(0,i) + "1" +binary_number.substr(i+1);
            }
            else{
                binary_number = binary_number.substr(0,i) + "0" +binary_number.substr(i+1);
            }
        }
        for (int i = binary_number.length() - 1;i>=0;i = i - 1){
            if (binary_number[i]=='0'){
                binary_number = binary_number.substr(0,i) + "1" +binary_number.substr(i+1);
                break;
            }
            else{
                binary_number = binary_number.substr(0,i) + "0" +binary_number.substr(i+1);
            }
        }
        return binary_number;
    }
    return 0;
}

int binReg_to_decInt(string str){
    int number = 0;
    int bit = 0;
    for (int i = str.length()-1;i>=0;i=i-1){
        if (str[i]=='1'){
            number = number + pow(2,bit);
            bit = bit + 1;
        }
        else{
            bit = bit + 1;
        }
    }
    return number;
}

int hexStr_to_dec(string s)
{
    int i=0,count,sum;

    count=s.length();
    sum=0;
    for(i=count-1;i>=0;i--)
    {
        if(s[i]>='0'&&s[i]<='9')
        {
            sum+=(s[i]-48)*pow(16,count-i-1);
        }
        else if(s[i]>='A'&&s[i]<='F')
        {
            sum+=(s[i]-55)*pow(16,count-i-1);
        }
    }
    return sum;
}

//all the functions of all the instructions as follows:
void mtlo_func(int* rs,int* lo){
        *lo = *rs;
}

void mthi_func(int* rs,int* hi){
        *hi = *rs;
}

void mflo_func(int* rd,int* lo){
        *rd = *lo;
}

void mfhi_func(int* rd,int* hi){
        *rd = *hi;
}

void tne_func(int* rs, int* rt,int pc){
        if (*rs != *rt){
                printf("exception occurs at %d",pc);
                exit(1);
        }
}

void teq_func(int* rs, int* rt,int pc){
        if (*rs == *rt){
                printf("exception occurs at %d",pc);
                exit(1);
        }
}

void tlt_func(int* rs, int* rt,int pc){
        if (*rs < *rt){
                printf("exception occurs at %d",pc);
                exit(1);
        }
}

void tltu_func(int* rs, int* rt,int pc){
        if (*rs < *rt){
                printf("exception occurs at %d",pc);
                exit(1);
        }
}

void tge_func(int* rs, int* rt,int pc){
        if (*rs >= *rt){
                printf("exception occurs at %d",pc);
                exit(1);
        }
}

void tgeu_func(int* rs, int* rt,int pc){
        if (*rs >= *rt){
                printf("exception occurs at %d",pc);
                exit(1);
        }
}

void teqi_func(int* rs,int imm,int pc){
        if (*rs == imm){
                printf("exception occurs at %d",pc);
                exit(1);
        }
}

void tnei_func(int* rs,int imm,int pc){
        if (*rs != imm){
                printf("exception occurs at %d",pc);
                exit(1);
        }
}

void tgei_func(int* rs,int imm,int pc){
        if (*rs >= imm){
                printf("exception occurs at %d",pc);
                exit(1);
        }
}

void tlti_func(int* rs,int imm,int pc){
        if (*rs < imm){
                printf("exception occurs at %d",pc);
                exit(1);
        }
}

void tltiu_func(int* rs,int imm,int pc){
        if (*rs < imm){
                printf("exception occurs at %d",pc);
                exit(1);
        }
}

void tgeiu_func(int* rs,int imm,int pc){
        if (*rs >= imm){
                printf("exception occurs at %d",pc);
                exit(1);
        }
}

void slt_func (int* rs, int* rt,int* rd){
        if (*rs < *rt){
                *rd = 1;
        }
        else{
                *rd = 0;
        }
}

void sltu_func (int* rs, int* rt,int* rd){
        if (*rs < *rt){
                *rd = 1;
        }
        else{
                *rd = 0;
        }
}

void slti_func(int* rs, int imm,int* rt){
        if (*rs < imm){
                *rt = 1;
        }
        else{
                *rt = 0;
        }
}

void sltiu_func(int* rs, int imm,int* rt){
        if (*rs < imm){
                *rt = 1;
        }
        else{
                *rt = 0;
        }
}

void sll_func (int shamt, int* rt,int* rd){
        string rtStr = dec_to_binary(*rt,32);
        string sub_rtStr = rtStr.substr(shamt);
        while (sub_rtStr.length()!=32){
                sub_rtStr = sub_rtStr + "0";
        }
        *rd = binStr_to_decInt(sub_rtStr);
}

void sllv_func (int* rs, int* rt,int* rd){
        string rtStr = dec_to_binary(*rt,32);
        string sub_rtStr = rtStr.substr(*rs);
        while (sub_rtStr.length()!=32){
                sub_rtStr = sub_rtStr + "0";
        }
        *rd = binStr_to_decInt(sub_rtStr);
}

void sra_func (int shamt, int* rt,int* rd){
        string rtStr = dec_to_binary(*rt,32);
        string place;
        if (rtStr[0] == '1'){
                place = "1";
        }
        else{
                place = "0";
        }
        string sub_rtStr = rtStr.substr(0,rtStr.length()-shamt);
        while (sub_rtStr.length()!=32){
                sub_rtStr = place + sub_rtStr;
        }
        *rd = binStr_to_decInt(sub_rtStr);
}

void srav_func (int* rs, int* rt,int* rd){
        string rtStr = dec_to_binary(*rt,32);
        string place;
        if (rtStr[0] == '1'){
                place = "1";
        }
        else{
                place = "0";
        }
        string sub_rtStr = rtStr.substr(0,rtStr.length()-*rs);
        while (sub_rtStr.length()!=32){
                sub_rtStr = place + sub_rtStr ;
        }
        *rd = binStr_to_decInt(sub_rtStr);
}

void srl_func (int shamt, int* rt,int* rd){
        string rtStr = dec_to_binary(*rt,32);
        string sub_rtStr = rtStr.substr(0,rtStr.length()-shamt);
        while (sub_rtStr.length()!=32){
                sub_rtStr =  "0" + sub_rtStr ;
        }
        *rd = binStr_to_decInt(sub_rtStr);
}

void srlv_func (int* rs, int* rt,int* rd){
        string rtStr = dec_to_binary(*rt,32);
        string sub_rtStr = rtStr.substr(0,rtStr.length()-*rs);
        while (sub_rtStr.length()!=32){
                sub_rtStr =  "0" + sub_rtStr ;
        }
        *rd = binStr_to_decInt(sub_rtStr);
}

void addi_func (int* rs, int imm,int* rt){
        *rt = *rs + imm;
}

void addiu_func (int* rs, int imm,int* rt){
        *rt = *rs + imm;
}

void add_func (int* rs, int* rt,int* rd){
        *rd = *rs + *rt;
}

void addu_func (int* rs, int* rt,int* rd){
        *rd = *rs + *rt;
}

int bne_func(int* rs,int *rt,int pc,int offset){
        if (*rs != *rt){
                pc = pc + (offset+1)*4;
        }
        else{
                pc = pc + 4;
        }
        return pc;
}

int beq_func(int* rs,int *rt,int pc,int offset){
        if (*rs == *rt){
                pc = pc + (offset+1)*4;
        }
        else{
                pc = pc + 4;
        }
        return pc;
}

int bgez_func(int* rs,int pc,int offset){
        if (*rs >= 0){
                pc = pc + (offset+1)*4;
        }
        else{
                pc = pc + 4;
        }
        return pc;
}

int bgtz_func(int* rs,int pc,int offset){
        if (*rs > 0){
                pc = pc + (offset+1)*4;
        }
        else{
                pc = pc + 4;
        }
        return pc;
}

int bgezal_func(int* rs,int pc,int offset){
        if (*rs >= 0){
                pc = pc + (offset+1)*4;
        }
        else{
                pc = pc + 4;
        }
        return pc;
}

int bltzal_func(int* rs,int pc,int offset){
        if (*rs < 0){
                pc = pc + (offset+1)*4;
        }
        else{
                pc = pc + 4;
        }
        return pc;
}

int bltz_func(int* rs,int pc,int offset){
        if (*rs < 0){
                pc = pc + (offset+1)*4;
        }
        else{
                pc = pc + 4;
        }
        return pc;
}


int blez_func(int* rs,int pc,int offset){
        if (*rs <= 0){
                pc = pc + (offset+1)*4;
        }
        else{
                pc = pc + 4;
        }
        return pc;
}


void sub_func(int* rs,int* rt,int* rd){
        *rd = *rs-*rt;
}

void subu_func(int* rs,int* rt,int* rd){
        *rd = *rs-*rt;
}


void lw_func(int* rs,int* rt,int offset){
        *rt = *rs+offset;
}

void lui_func(int* rt,int imm){
        int lowWord = imm % 65535;
        int new_word = lowWord * 65536;
        *rt = new_word;
}

void ori_func(int* rs,int imm,int* rt){
        string rsStr = dec_to_binary(*rs,32);
        string immStr = dec_to_binary(imm,32);
        string rtStr;
        for (int i = 0;i<32;i=i+1){
                if ((rsStr[i]=='0')&&(immStr[i]=='0')){
                        rtStr = rtStr + "0";
                }
                else{
                        rtStr = rtStr + "1";
                }
        }
        *rt = binStr_to_decInt(rtStr);
}

void xori_func(int* rs,int imm,int* rt){
        string rsStr = dec_to_binary(*rs,32);
        string immStr = dec_to_binary(imm,32);
        string rtStr;
        for (int i = 0;i<32;i=i+1){
                if ((rsStr[i]=='0')&&(immStr[i]=='0')){
                        rtStr = rtStr + "0";
                }
                else if ((rsStr[i]=='1')&&(immStr[i]=='1')){
                        rtStr = rtStr + "0";
                }
                else{
                        rtStr = rtStr + "1";
                }
        }
        *rt = binStr_to_decInt(rtStr);
}

void andi_func(int* rs,int imm,int* rt){
        string rsStr = dec_to_binary(*rs,32);
        string immStr = dec_to_binary(imm,32);
        string rtStr;
        for (int i = 0;i<32;i=i+1){
                if ((rsStr[i]=='1')&&(immStr[i]=='1')){
                        rtStr = rtStr + "1";
                }
                else{
                        rtStr = rtStr + "0";
                }
        }
        *rt = binStr_to_decInt(rtStr);
}


void and_func(int* rs,int* rt,int* rd){
        string rsStr = dec_to_binary(*rs,32);
        string rtStr = dec_to_binary(*rt,32);
        string rdStr;
        for (int i = 0;i<32;i=i+1){
                if ((rsStr[i]=='1')&&(rtStr[i]=='1')){
                        rdStr = rdStr + "1";
                }
                else{
                        rdStr = rdStr + "0";
                }
        }
        *rd = binStr_to_decInt(rdStr);
}

void or_func(int* rs,int* rt,int* rd){
        string rsStr = dec_to_binary(*rs,32);
        string rtStr = dec_to_binary(*rt,32);
        string rdStr;
        for (int i = 0;i<32;i=i+1){
                if ((rsStr[i]=='0')&&(rtStr[i]=='0')){
                        rdStr = rdStr + "0";
                }
                else{
                        rdStr = rdStr + "1";
                }
        }
        *rd = binStr_to_decInt(rdStr);
}

void xor_func(int* rs,int* rt,int* rd){
        string rsStr = dec_to_binary(*rs,32);
        string rtStr = dec_to_binary(*rt,32);
        string rdStr;
        for (int i = 0;i<32;i=i+1){
                if ((rsStr[i]=='0')&&(rtStr[i]=='0')){
                        rdStr = rdStr + "0";
                }
                else if ((rsStr[i]=='1')&&(rtStr[i]=='1')) {
                        rdStr = rdStr + "0";
                }
                else{
                        rdStr = rdStr + "1";
                }
        }
        *rd = binStr_to_decInt(rdStr);
}

void nor_func(int* rs,int* rt,int* rd){
        string rsStr = dec_to_binary(*rs,32);
        string rtStr = dec_to_binary(*rt,32);
        string rdStr;
        for (int i = 0;i<32;i=i+1){
                if ((rsStr[i]=='0')&&(rtStr[i]=='0')){
                        rdStr = rdStr + "1";
                }
                else{
                        rdStr = rdStr + "0";
                }
        }
        *rd = binStr_to_decInt(rdStr);
}

int jal_func(int pc, int* ra,int target){
        *ra = pc + 4;
        pc = 4194304 + (target * 4);
        return pc;
}

int j_func(int pc,int target){
        pc = 4194304 + (target * 4);
        return pc;
}

int jalr_func(int pc,int *rs,int* rd){
        *rd = pc + 4;
        int address = *rs;
        return address;
}

int jr_func(int* rs){
        int pc = *rs;
        return pc;
}

void div_func (int* rs,int* rt,int* lo,int* hi){
        *lo = *rs / *rt;
        *hi = *rs % *rt;
}

void divu_func (int* rs,int* rt,int* lo,int* hi){
        *lo = *rs / *rt;
        *hi = *rs % *rt;
}

void mult_func (int* rs,int* rt,int * lo,int* hi){
        long result = (*rs) * (*rt);
        string result_string = long_dec_to_binary(result,64);
        string string_hi = result_string.substr(0,32);
        string string_lo = result_string.substr(32,32);
        *hi = binStr_to_decInt(string_hi);
        *lo = binStr_to_decInt(string_lo);
}

void multu_func (int* rs,int* rt,int * lo,int* hi){
        long result = (*rs) * (*rt);
        string result_string = long_dec_to_binary(result,64);
        string string_hi = result_string.substr(0,32);
        string string_lo = result_string.substr(32,32);
        *hi = binStr_to_decInt(string_hi);
        *lo = binStr_to_decInt(string_lo);
}
