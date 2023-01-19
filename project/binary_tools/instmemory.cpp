#include "instmemory.h"

instMemory::instMemory(){}
instMemory::~instMemory(){}

bool instMemory::isEmpty() {
    return lineNum.empty() && inst.empty();
}

int instMemory::getLineNum(int index) const {
    return lineNum[index];
}

const QString instMemory::getInst(int index) const {
    return inst[index];
}

void instMemory::push_back(int index) {
    lineNum.push_back(index);
}

void instMemory::setInst(QStringList list) {
    inst = list;
}

int instMemory::getSize() const {
    return static_cast<int>(lineNum.size());
}
