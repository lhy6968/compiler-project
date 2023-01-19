#ifndef LINKER_H
#define LINKER_H

#include "objassM.h"
#include <initializer_list>

class linker
{
public:
    objInfo * objifpt;
    int txtbase_temp;
    int database_temp;

    linker();
    linker(initializer_list<objInfo> objlist);
    ~linker();
    void linkNewFile(const objInfo * srcpt);

    template<typename T>
    vector<T> shiftAddr(vector<T> srcvec, int initialBase, int tembase);
};

linker::linker(){
    objifpt = new objInfo;
    txtbase_temp = TEXT_BASE;
    database_temp = DATA_BASE;
}

linker::linker(initializer_list<objInfo> objlist){
    objifpt = new objInfo;
    txtbase_temp = TEXT_BASE;
    database_temp = DATA_BASE;
    for (auto beg = objlist.begin(); beg != objlist.end(); ++beg){
        linkNewFile(beg);
    }
}

linker::~linker(){
    delete [] objifpt;
}

void relocateOffset(objInfo & objif, int addr, int label_addr){
    for (int i = 0; i < (int) objif.textseg.size(); i++){
        if (objif.textseg[i].addr == addr) {
            int offset = (label_addr - objif.textseg[i].addr) / 4;
            objif.textseg[i].instr = ((objif.textseg[i].instr >> 16) << 16) + offset;
            return;
        }
    }
}

void relocateTarget(objInfo & objif, int addr, int label_addr){
//    cout << rifpiece.depend << " " << hex << label_addr << " " << hex << addr-4 <<endl;
    for (int i = 0; i < (int) objif.textseg.size(); i++){
        if (objif.textseg[i].addr == addr) {
            int target = (label_addr - TEXT_BASE) / 4;
            objif.textseg[i].instr = ((objif.textseg[i].instr >> 26) << 26) + target;
            return;
        }
    }
}

void linker::linkNewFile(const objInfo * srcpt){
    objifpt->datasize += srcpt->datasize;
    objifpt->textsize += srcpt->textsize;

    vector<textSeg> srctext = shiftAddr(srcpt->textseg, 0, txtbase_temp);
    vector<dataTab> srcdatatab = shiftAddr(srcpt->datatab, 0, database_temp);
    vector<sTable> srcstab = shiftAddr(srcpt->stable, 0, txtbase_temp);
    vector<rInfo> srcrinfo = shiftAddr(srcpt->rinfo, 0, txtbase_temp);
    vector<string> srcdataseg = srcpt->dataseg;
    objifpt->dataseg.insert(objifpt->dataseg.end(), srcdataseg.begin(), srcdataseg.end());
    objifpt->textseg.insert(objifpt->textseg.end(), srctext.begin(), srctext.end());
    objifpt->datatab.insert(objifpt->datatab.end(), srcdatatab.begin(), srcdatatab.end());
    objifpt->stable.insert(objifpt->stable.end(), srcstab.begin(), srcstab.end());
    objifpt->rinfo.insert(objifpt->rinfo.end(), srcrinfo.begin(), srcrinfo.end());

    for (int i = 0; i < (int)objifpt->rinfo.size(); i++){
        rInfo rifpiece = objifpt->rinfo[i];
        if (reloType[rifpiece.instrType] != I6) {
            int addr = rifpiece.addr;
            int label_addr = (*objifpt).locateLabel(rifpiece.addr, rifpiece.instrType, rifpiece.depend, 0);
            if (label_addr != -1) {
                if (reloType[rifpiece.instrType] == J) relocateTarget(*objifpt, addr - 4, label_addr);
                else relocateOffset(*objifpt, addr, label_addr);
                objifpt->rinfo.erase(objifpt->rinfo.begin() + i);
                i--;
            }
        }
    }
    txtbase_temp += srcpt->textsize;
    database_temp += srcpt->datasize;
}

template <typename T>
vector<T> linker::shiftAddr(vector<T> srcvec, int initialBase, int tembase){
    for (int i = 0; i < (int) srcvec.size(); i++){
        srcvec[i].addr = srcvec[i].addr - initialBase + tembase;
    }
    return srcvec;
}


#endif // LINKER_H
