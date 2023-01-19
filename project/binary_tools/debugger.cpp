#include "debugger.h"
#include <string>
#include <vector>
using namespace std;

Debugger::Debugger()
{
    regs = new Reg();
    current_ptr = 0;
}

void Debugger::setInst(const instMemory& instructions) {
    instMem = instructions;
    size = instMem.getSize();
}

void Debugger::getreg(Reg * regsrc){
    for (int i = 0; i < 32; i++){
        regs->registers[i]->identifier = regsrc->registers[i]->identifier;
        regs->registers[i]->value = regsrc->registers[i]->value;
    }

    regs->HI->identifier = regsrc->HI->identifier;
    regs->HI->value = regsrc->HI->value;
    regs->LO->identifier = regsrc->LO->identifier;
    regs->LO->value = regsrc->LO->value;
}

int Debugger::init() {
    hi_void = malloc(4);
    lo_void = malloc(4);
    registers = malloc(128);

    ifstream in;
    string f_path = "objlinker.txt";
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
                RVA_found.push_back(string(txtList[it].substr(p1,p2)));//find and store label

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

    vector<string>::size_type index = RVA_found.size()-1;
    string biggest_RVA = RVA_found[index];
    int_biggest_RVA = hexStr_to_dec(biggest_RVA);

    //use int order to record the offset
    int order = 0;
    for (int i = 0;i<int(machineList.size());i=i+1){
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
    *(register1+1) = 0;
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
    *(register1+24) = 0;
    *(register1+25) = 0;
    *(register1+26) = 0;
    *(register1+27) = 0;

    //use int sbrk_number to store the address of sbrk space
    sbrk_number = 0;

    //use int data_end to store the fake address of the end of static data
    data_end = new_address(mm,m);

    //use int new_number to store the beginning of the fake address
    new_number = new_address(m,m);

    //initializes the value in the special registers
    *(register1+29) = new_number + 6291456;
    *(register1+30) = new_number + 6291456;
    *(register1+28) = new_number + 1048576;
    *(register1+31) = 0;

    //initializes the value of PC
    pc = new_number;

    //initializes the value in the zero registers
    zero1 = (int *)registers;
    *zero1 = 0;

    int *hi_int = (int * )hi_void;
    * hi_int = 0;
    int *lo_int = (int * )lo_void;
    * lo_int = 0;

    return 0;
}
