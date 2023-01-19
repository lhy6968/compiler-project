#include "register.h"

Register::Register(std::string idsrc, int valuesrc){
    identifier = idsrc;
    value = valuesrc;
}

Reg::Reg()
{
    for(int i = 0; i < 32; i++) {
        Register * reg = new Register("$"+rgsName[i], 0);
        registers.push_back(reg);

    }
    PC = new Register("PC", 0x400000);
    HI = new Register("HI", 0);
    LO = new Register("LO", 0);
}

Reg::~Reg() {
    delete PC;
    delete HI;
    delete LO;
    for(int i = 0; i < 32; i++) {
        delete registers[i];
    }
}

void Reg::fresh() {
    for(int i = 0; i < 32; i++) {
        setVal(i, 0);
    }
    PC->value = 0x400000;
    HI->value = 0;
    LO->value = 0;
}

void Reg::setVal(int index, int value) {
    if(index == 32)
        return;
    registers[index]->value = value;
}
