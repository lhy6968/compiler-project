#ifndef REGISTER_H
#define REGISTER_H
#include <vector>
#include <string>

class Register{
public:
    std::string identifier = "";
    int value = 0;
    Register(std::string idsrc, int valuesrc);
};

class Reg {
public:
    Reg();
    ~Reg();
    void reset();
    void fresh();
    void setVal(int index, int val);
    std::string rgsName[32] = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
                               "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"};
    std::vector<Register * > registers;
    Register * PC;
    Register * HI;
    Register * LO;
};



#endif // REGISTER_H
