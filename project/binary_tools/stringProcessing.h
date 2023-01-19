#ifndef STRINGPROCESSING_H
#define STRINGPROCESSING_H

// This header file offer the functions to deal with one line of MIPS command.

#include <string>

using namespace std;

// to clear the comments in the string.
void clearComment(string & str){
    int comment_index = str.find("#");
	if (comment_index != -1) str = str.substr(0, comment_index);
}


// to clear all the spaces in the head and tail of the string.
string clearHeadTailSpace(string & str){
    if (str.empty()){
        return str;
    }
    str.erase(0, str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" ")+1);
    str.erase(0, str.find_first_not_of("\n"));
    str.erase(str.find_last_not_of("\n")+1);
    str.erase(0, str.find_first_not_of("\r"));
    str.erase(str.find_last_not_of("\r")+1);
    str.erase(0, str.find_first_not_of("\t"));
    str.erase(str.find_last_not_of("\t")+1);
    return str;
}


// to erase the label in one line of MIPS command.
string clearLabel(string & str){
    int flag = str.find(":");
    if (flag != -1) str = str.substr(flag+1);
    return clearHeadTailSpace(str);
}

// to slice the instruction from the line of MIPS command.
string instruction(string & line){
    int instr_f = line.find(" ");
    string instr = line.substr(0, instr_f);
    line = line.substr(instr_f + 1);
    return clearHeadTailSpace(instr);
}


// to slice one parameter (registers or constants) from the line of command.
string slicePara(string & line){
    string para;
    int para_f = line.find(",");
    if (para_f != -1){
        para = line.substr(0, para_f);
        line = line.substr(para_f+1);
    } else {
        para = line.substr(0);
        line = "";
    }   
    return clearHeadTailSpace(para);
}

#endif // STRINGPROCESSING_H

