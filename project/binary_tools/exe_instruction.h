#ifndef EXE_INSTRUCTION_H
#define EXE_INSTRUCTION_H

#include<string>
#include<vector>
using namespace std;

//execute instructions line by line
vector<int> exe_all_instruct(int pc, int new_number, vector<string> machineList, int *int_memory, void *registers, void *hi_void,
                      void *lo_void, char * char_memory, int sbrk_number, int data_end, int *zero1);

//execute one instruction
vector<int> exe_one_instruct(int pc, int new_number, int *int_memory, void *registers, void *hi_void,
                      void *lo_void, char * char_memory, int sbrk_number, int data_end, int *zero1, int line_num, bool &finish);

//get data in registers
vector<int> get_registers(void *registers, void *hi_void, void *lo_void, int pc);

#endif // EXE_INSTRUCTION_H
