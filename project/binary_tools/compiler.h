#ifndef COMPILER_H
#define COMPILER_H
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>
#include <CodeGen.h>

using namespace std;

// The roots of syntax trees include main and other function definitions
extern vector<Node>roots;
// Varibles that are declared in source code
extern vector<string>variables;


// Do the lexical analysis
void lexer(string file);

// Judge if the token is existed in one line of code
bool existed(string token, string line);

// Build a syntax tree by connecting chidren node with the root
void buildTree(string line, Node *&node);

// Mainly used in arithmetic expression, to check '+' '-' '*' '/'.
void symbolCheck(string line, Node *&node);

// Judge the type of a symple expression
void varType(string expr, Node *&node);

// Connect the nodes of variables and their types
void varCon(string type, string line, Node *&node);

// Eliminate invalid white space
void trim(string &code);

// Call the compiler
void compiler(string str);

#endif // COMPILER_H
