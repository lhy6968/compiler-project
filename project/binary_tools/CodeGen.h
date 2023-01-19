#ifndef CODEGEN_H
#define CODEGEN_H

#include <string>
#include <vector>

struct Node{
    std::string type, data;
    std::vector<Node> leaves;
    std::string symbol;
    Node *expr1 = NULL;
    Node *expr2 = NULL;
};


void translation(std::vector<Node> &roots, std::string str);


#endif // CODEGEN_H
