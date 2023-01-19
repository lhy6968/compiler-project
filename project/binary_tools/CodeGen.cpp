#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <CodeGen.h>
using namespace std;


// Container for static data
struct tag{
    string label;       // Virtual label for .data part
    string name;        // Variable name / string content
    string type;        // "int" / "string"
    int address;
};


// Translate an integer into string
string intToString(int a) {
    stringstream ss;
    ss << a;
    return ss.str();
}


// Translate a string into integer
int strToInt(string s) {
    stringstream ss;
    int a;
    ss << s;
    ss >> a;
    return a;
}


// Load all the static data from the AST into a vector;
void loadStatic(Node *&root, vector<tag> &staticData) {
    int numCount = 1;
    int strCount = 1;

    for (Node i:root->leaves) {
        if (i.expr1 != NULL) {
            if (i.expr1->type == "int") {           // put int into bucket
                tag newtag;
                newtag.type = "int";
                newtag.name = i.expr1->expr1->data;
                newtag.label = "num" + intToString(numCount);
                newtag.address = 0;
                staticData.push_back(newtag);
                numCount += 1;
            }
            else if (i.expr1->type == "print") {    // put print str into bucket
                if (i.expr1->expr1->type == "string") {
                    tag newtag;
                    newtag.type = "string";
                    newtag.name = i.expr1->expr1->data;
                    newtag.label = "str" + intToString(strCount);
                    newtag.address = 0;
                    staticData.push_back(newtag);
                    strCount += 1;
                }
            }
        }
    }
}


// Write all the static data into file, according to the syntax of MIPS code
void writeStatic(vector<tag> &staticData, stringstream &ss, int &address) {
    ss << ".data" << endl;
    for (int i=0; i<static_cast<int>(staticData.size()); i++) {
        if (staticData[i].type == "int") {
            staticData[i].address = address;
            ss << staticData[i].label << ": .word 0" << endl;
            address += 4;
        }
        else if (staticData[i].type == "string") {
            staticData[i].address = address;
            ss << staticData[i].label << ": .asciiz \"" << staticData[i].name << "\"" << endl;
            address += ((staticData[i].name.size()+1+3)/4)*4;
        }
    }
}


// Find the virtual address of an int variable and return.
// If failed, return -1
int addrIntVar(string s, vector<tag> &staticData) {
    for (tag t:staticData) {
        if (t.type == "int" && t.name == s) {
            return t.address;
        }
    }
    return -1;
}

// Find the virtual address of a string and return.
// If failed, return -1
int addrStrVar(string s, vector<tag> &staticData) {
    for (tag t:staticData) {
        if (t.type == "string" && t.name == s) {
            return t.address;
        }
    }
    return -1;
}


// Judge whether a string only contains digits.
bool strIsDigit(string s) {
    for (char ch:s) {
        if (not isdigit(ch)) return false;
    }
    return true;
}


// MIPS code for input function, saves to a variable
void input(int addr, int &addrCnt, stringstream &ss) {
    ss << "addi $v0, $zero, 5" << endl;
    ss << "syscall" << endl;
    ss << "sw $v0, " << intToString(addr-0x500000) << "($gp)" << endl;
    addrCnt += 12;
}


// MIPS code for input and saves to a register
void inputReg(string reg, int &addrCnt, stringstream &ss) {
    ss << "addi $v0, $zero, 5" << endl;
    ss << "syscall" << endl;
    ss << "addi " << reg << ", $zero, $v0" << endl;
    addrCnt += 12;
}


// MIPS code for print. Able to print out a variable, a string or a number.
void print(int addr, int &addrCnt, stringstream &ss, bool strflag, bool numFlag) {
    if (strflag) {
        ss << "lui $a0, 80" << endl;
        ss << "ori $a0, $a0, " << intToString(addr-0x500000) << endl;
        ss << "addi $v0, $zero, 4" << endl;
        ss << "syscall" << endl;
        addrCnt += 16;
    }
    else if (numFlag) {
        ss << "addi $a0, $zero, " << intToString(addr) << endl;
        ss << "addi $v0, $zero, 1" << endl;
        ss << "syscall" << endl;
        addrCnt += 12;
    }
    else {
        ss << "lw $a0, " << intToString(addr-0x500000) << "($gp)" << endl;
        ss << "addi $v0, $zero, 1" << endl;
        ss << "syscall" << endl;
        addrCnt += 12;
    }
}


// MIPS code for print. Able to print an integer from a register.
void printReg(string reg, int &addrCnt, stringstream &ss) {
    ss << "add $a0, $zero, " << reg << endl;
    ss << "addi $v0, $zero, 1" << endl;
    ss << "syscall" << endl;
    addrCnt += 12;
}


// MIPS code for load word. From memory to a register
string lw(int addr, int &addrCnt, vector<string> &tempRegs, stringstream &ss) {     // load word
    string reg;
    reg = tempRegs[tempRegs.size()-1];
    tempRegs.pop_back();
    ss << "lw " << reg << ", " << intToString(addr-0x500000) << "($gp)" << endl;
    addrCnt += 4;
    return reg;
}


// MIPS code for load immediate number into a register.
string li(string imm, int &addrCnt, vector<string> &tempRegs, stringstream &ss) {   // load immediate
    string reg;
    reg = tempRegs[tempRegs.size()-1];
    tempRegs.pop_back();
    ss << "addi " << reg << ", $zero, " << imm << endl;
    addrCnt += 4;
    return reg;
}


// MIPS code to free previously assigned register, by appending
// it back to the vector of registers.
void freeReg(string reg, vector<string> &tempRegs) {
    if (reg != "$s0" && reg != "$s1") {
        tempRegs.push_back(reg);
    }
}


// MIPS code to assign a register to a variable
void assign(int addr, string reg, int &addrCnt, stringstream &ss) {
    ss << "sw " << reg << ", " << intToString(addr-0x500000) << "($gp)" << endl;
    addrCnt += 4;
}


// MIPS code to assign a register to a register
void assignReg(string lreg, string rreg, int &addrCnt, stringstream &ss) {
    ss << "add " << lreg << ", $zero, " << rreg << endl;
    addrCnt += 4;
}


// MIPS code for calculation
// Including addition, subtraction, multiplication and division between integers.
void calc(char symbol, string rd, string rs, string rt, int &addrCnt, stringstream &ss) {
    string op;

    switch (symbol) {
        case '+': op = "add"; break;
        case '-': op = "sub"; break;
        case '*': op = "mult"; break;
        case '/': op = "div"; break;
    }
    if (symbol == '+' || symbol == '-') {       // add & sub
        ss << op << " " << rd << ", " << rs << ", " << rt << endl;
        addrCnt += 4;
    }
    else {                                      // mult & div
        ss << op << " " << rs << ", " << rt << endl;
        ss << "mflo " << rd << endl;
        addrCnt += 8;
    }
}


// MIPS code to exit the program.
void ext(int &addrCnt, stringstream &ss) {      // exit
    ss << "addi $v0, $zero, 10" << endl;
    ss << "syscall" << endl;
    addrCnt += 8;
}


// MIPS code for jump to the address in the register
void jr(int &addrCnt, stringstream &ss) {
    ss << "jr $ra" << endl;
    addrCnt += 4;
}


// MIPS code for jump and link
void jal(string func, int &addrCnt, stringstream &ss) {
    ss << "jal " << func << endl;
    addrCnt += 4;
}


// Tree traversal
// Traversing the AST and translate it into MIPS code
string treeTrav(Node *&root, int &address, vector<tag> &staticData, stringstream &ss, vector<string> &tempRegs, vector<string> parameters) {
    string ldata, rdata;

    if (root->expr1 != NULL) {
        ldata = treeTrav(root->expr1, address, staticData, ss, tempRegs, parameters);
    }

    if (root->expr2 != NULL) {
        rdata = treeTrav(root->expr2, address, staticData, ss, tempRegs, parameters);
    }

    if (root->expr1 != NULL || root->expr2 != NULL) {           // Code generation starts here
        string rs, rt, rd;
        char symbol = root->symbol[0];
        rs = rt = "$zero";
                                                        // function
        if (root->type != "int" && root->type != "input" && root->type != "print" && root->type != "variable" && root->type != "parameter" && root->type != "expression" && root->type != "string" && root->type != "factor" && root->type != "") {
            if (ldata != "") {
                assignReg("$s0", ldata, address, ss);
                freeReg(ldata, tempRegs);
            }
            if (rdata != "") {
                assignReg("$s1", rdata, address, ss);
                freeReg(rdata, tempRegs);
            }
            jal(root->type, address, ss);
        }
        else if (root->type == "input") {                // input
            int addr;
            addr = addrIntVar(root->expr1->data,staticData);
            if (ldata == "$s0" || ldata == "$s1") {
                inputReg(ldata, address, ss);
            }
            else {
                if (addr == -1) {
                    cout << "Input variable does not exist." << endl;
                    exit(0);
                }
                input(addr, address, ss);
            }
            freeReg(ldata, tempRegs);
            return "";
        }
        else if (root->type == "print") {           // print
            int addr;
            bool strFlag = true;
            bool numFlag = false;
            addr = addrStrVar(root->expr1->data, staticData);
            if (ldata == "$s0" || ldata == "$s1") {
                printReg(ldata, address, ss);
                freeReg(ldata, tempRegs);
            }
            else {
                if (addr == -1) {
                    strFlag = false;
                    addr = addrIntVar(root->expr1->data,staticData);
                    freeReg(ldata, tempRegs);
                    if (addr == -1) {
                        numFlag = true;
                        if (strIsDigit(root->expr1->data)) {
                            cout << "Input variable does not exist." << endl;
                            exit(0);
                        addr = strToInt(root->expr1->data);
                        }
                    }
                }
                print(addr, address, ss, strFlag, numFlag);
            }
            return "";
        }
        else if (symbol == '=') {                   // assign
            int addr;

            addr = addrIntVar(root->expr1->data, staticData);
            if (ldata == "$s0" || ldata == "$s1") {
                assignReg(ldata, rdata, address, ss);
            }
            else {
                if (addr == -1) {
                    cout << "Assigned variable does not exist." << endl;
                    exit(0);
                }
                assign(addr, rdata, address, ss);
            }
            freeReg(ldata, tempRegs);
            freeReg(rdata, tempRegs);
            return "";
        }
        else if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/') {
            string reg;

            reg = tempRegs[tempRegs.size()-1];
            tempRegs.pop_back();
            calc(symbol, reg, ldata, rdata, address, ss);
            freeReg(ldata, tempRegs);
            freeReg(rdata, tempRegs);
            return reg;
        }
    }
    else {
        int addr;
        string reg;
        bool isPar = false;
        int num;

        if (root->type != "int" && root->type != "input" && root->type != "print" && root->type != "variable" && root->type != "parameter" && root->type != "expression" && root->type != "string" && root->type != "factor" && root->type != "") {
            jal(root->type, address, ss);
        }
        else {
            if (parameters.size() == 1) {
                if (root->data == parameters[0]) {
                    isPar = true;
                    num = 0;
                }
            }
            else if (parameters.size() == 2) {
                if (root->data == parameters[0]) {
                    isPar = true;
                    num = 0;
                }
                else if (root->data == parameters[1]) {
                    isPar = true;
                    num = 1;
                }
            }

            if (isPar) {
                reg = "$s" + intToString(num);
            }
            else if ((addr = addrIntVar(root->data, staticData)) != -1) {   // variable
                reg = lw(addr, address, tempRegs, ss);
            }
            else if ((addr = addrStrVar(root->data, staticData)) != -1) {   // string
                ;
            }
            else if (root->data != ""){                                     // imm
                if (strIsDigit(root->data)) {
                    reg = li(root->data, address, tempRegs, ss);
                }
                else {
                    cout << "data: " << root->data << endl;
                    cout << "Syntax error." << endl;
                    exit(0);
                }
            }
            return reg;
        }
    }
    return "";
}

void deleteNode(Node* n) {
    Node *lnode;
    Node *rnode;

    if (n->expr1 != NULL) {
        lnode = n->expr1;
        n->expr1 = NULL;
        deleteNode(lnode);
    }

    if (n->expr2 != NULL) {
        rnode = n->expr2;
        n->expr2 = NULL;
        deleteNode(lnode);
    }

    delete n;
}


// Essential procedure to write the MIPS code
void writeCode(vector<Node> &roots, vector<tag> &staticData, int &address, stringstream &ss) {
    vector<string> tempRegs = {"$t9", "$t8", "$t7", "$t6", "$t5", "$t4", "$t3", "$t2", "$t1", "$t0"};
    Node *root;

    ss << ".text" << endl;

    for (Node i:roots) {
        vector<string> parameters;
        root = &i;

        if (root->expr1->type == "parameter") {
            parameters.push_back(root->expr1->data);
        }

        if (root->expr2->type == "parameter") {
            parameters.push_back(root->expr2->data);
        }

        if (root->type == "main") {
            ss << "main: " << endl;
        }
        else if (root->type == "def") {
            ss << root->symbol << ": " << endl;
        }

        for (Node code:root->leaves) {
            Node *ptr = &code;
            if (ptr->expr1 != NULL) {
                if (ptr->expr1->type != "int") {
                    treeTrav(ptr, address, staticData, ss, tempRegs, parameters);
                    ss << endl;
                }
//                deleteNode(ptr);
            }
            else {
//                delete ptr;
            }
        }

        if (root->type == "main") {
            ext(address, ss);
            ss << endl;
        }
        else if (root->type == "def") {
            jr(address, ss);
            ss << endl;
        }

        delete root;
    }
}


// Code generation, including static data part and text part
// Receiving the AST and translate it into MIPS assembly code
void translation(vector<Node> &roots, string str) {
    string filename;

    filename = str.substr(0, str.find_last_of('.')) + ".asm";
    ofstream fout;
    fout.open(filename);

    stringstream tempStream;
    int address = 0x500000;
    int codeAddr = 0x400000;
    int staticSize;
    int textSize;
    string line;

    vector<tag> staticData;

    for (Node i:roots) {
        Node *n;
        n = &i;
        loadStatic(n, staticData);
    }

    writeStatic(staticData, tempStream, address);
    staticSize = address - 0x500000;

    writeCode(roots, staticData, codeAddr, tempStream);
    textSize = codeAddr - 0x400000;

    fout << "# text = " + intToString(textSize) + " B" << endl;
    fout << "# static = " + intToString(staticSize) + " B" << endl;
    fout << tempStream.str();

    fout.close();
}
