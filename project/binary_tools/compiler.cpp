#include "compiler.h"
#include <CodeGen.h>

vector<Node>roots;
vector<string>variables;

// Judge if the token is existed in the line of expression
bool existed(string token, string line){
    if (line.find(token)!=string::npos) return true;
    else return false;
}

// Erase white space
void trim(string &code){
    if (existed("'", code)){
       int first = code.find_first_of("'");
       int second = code.find_last_of("'");
       for (int i = 0; i < static_cast<int>(code.size()); i++){
           if (code[i] == ' ' && (i < first || i > second)){
               code.erase(i, 1);
           }
       }
    }
    else{
        while (true){
            if (existed(" ", code)){
                code.erase(code.find(" "), 1);
            }
            else break;
        }
    }
}

// Do the lexical analysis line by line
void lexer(string file){
    ifstream infile;
    infile.open(file.c_str());
    if(infile.fail()) cout << "fail" << endl;
    string line;
    bool isMain;

    while (getline(infile, line)){
        if (line == "main"){
            isMain = true;
            Node *mainTree = new Node;
            mainTree -> type = "main";
            mainTree -> expr1 = new Node;
            mainTree -> expr2 = new Node;
            mainTree -> expr1 -> type = "NA";
            mainTree -> expr2 -> type = "NA";
            roots.push_back(*mainTree);
        }

        else if (line.substr(0,3) == "def"){
            isMain = false;
            Node *defTree = new Node;
            defTree->type = "def";
            trim(line);
            defTree -> symbol = line.substr(4, line.find('}')-4);

            if (existed(",", line)){
                // 2 parameters
                defTree -> expr1 = new Node;
                defTree -> expr2 = new Node;
                defTree -> expr1 -> type = "parameter";
                defTree -> expr2 -> type = "parameter";
                defTree -> expr1 -> data = line.substr(line.find('}')+1, line.find(',')-line.find('}')-1);
                defTree -> expr2 -> data = line.substr(line.find(',')+1);
            }

            else if (line.find('}') !=line.size()-1){
                // 1 parameter
                defTree -> expr1 = new Node;
                defTree -> expr2 = new Node;
                defTree -> expr1 -> type = "parameter";
                defTree -> expr2 -> type = "NA";
                defTree -> expr1 -> data = line.substr(line.find('}')+1);
            }
            else {
                // 0 parameter
                defTree -> expr1 = new Node;
                defTree -> expr2 = new Node;
                defTree -> expr1 -> type = "NA";
                defTree -> expr2 -> type = "NA";
            }
            roots.push_back(*defTree);

        }

        else if (isMain == false){
            trim(line);
            Node *defTree = &roots[roots.size()-1];
            Node *codeLine = new Node;
            buildTree(line, codeLine);
            defTree -> leaves.push_back(*codeLine);
        }
        else if (isMain == true){
            Node *mainTree = &roots[roots.size()-1];
            trim(line);
            Node *codeLine = new Node;
            buildTree(line, codeLine);
            mainTree -> leaves.push_back(*codeLine);
        }
    }

    infile.close();
}

//Check symbols
void symbolCheck(string line, Node *&node, string symbol){
    string lexp, rexp;
    lexp = line.substr(0, line.find(symbol));
    rexp = line.substr(line.find(symbol)+1);
    node->expr1 = new Node;
    node->expr2 = new Node;
    node->expr1->data = lexp;
    if(existed("+", lexp) || existed("-", lexp) || existed("*", lexp) || existed("/", lexp)){
        node->expr1->type = "expression";
        buildTree(node->expr1->data, node->expr1);
    }
    else node->expr1->type = "factor";
    node->expr2->data = rexp;
    if(existed("+", rexp) || existed("-", rexp) || existed("*", rexp) || existed("/", rexp)){
        node->expr2->type = "expression";
        buildTree(node->expr2->data, node->expr2);
    }
    else node->expr2->type = "factor";
    node->symbol = symbol;
}

// Judge the type of a certain expression
void varType(string expr, Node *&node){
    if (existed("'", expr)){
       node->type = "string";
       node->data = expr.substr(1, expr.size()-2);
    }
    else if (find(variables.begin(), variables.end(), expr) != variables.end()){
       node->type = "variable";
       node->data = expr;
    }
    else{
       node->type = "int";
       node->data = expr;
    }
}

// Connect the type of variable and the name of variable
void varCon(string func, string line, Node *&node ){    
    if(existed(",", line)){
        //2 parameters
        string var1 = line.substr(line.find('}')+1, line.find(',')-line.find('}')-1);
        string var2 = line.substr(line.find(',')+1);
        node->expr1 = new Node;
        node->expr1->type = func;
        node->expr1->expr1 = new Node;
        node->expr1->expr2 = new Node;
        varType(var1, node->expr1->expr1);
        varType(var2, node->expr1->expr2);
    }
    else if (line.find('}') == line.size()-1){
        // no parameter
        node->expr1 = new Node;
        node->expr1->type = func;
    }
    else{
        // 1 parameter
        string var = line.substr(func.size()+2);
        node->expr1 = new Node;
        node->expr1->type = func;
        node->expr1->expr1 = new Node;
        varType(var, node->expr1->expr1);
    }
}

// Build the syntax tree by manipulating through different symbols
void buildTree(string line, Node *&node){
    string lexp, rexp;
    if (existed("{", line)){
        string func = line.substr(1, line.find('}')-1);
        varCon(func, line, node);
    }
    else if (existed("=", line)){
        lexp = line.substr(0, line.find('='));
        rexp = line.substr(line.find('=')+1);
        node->expr1 = new Node;
        node->expr2 = new Node;
        node->expr1->data = lexp;
        node->expr1->type = "variable";
        variables.push_back(lexp);
        node->expr2->data = rexp;
        if(existed("(", rexp) || existed("+", rexp) || existed("-", rexp) ||
                existed("*", rexp) || existed("/", rexp)){
            node->expr2->type = "expression";
            buildTree(node->expr2->data, node->expr2);
        }
        else node->expr2->type = "factor";
        node->symbol = "=";
    }

    else if (existed("(", line)){
        string lexp, rexp, cexp;
        stack<char> brackets;
        bool flag = false;
        if (line.find('(') == 0 && line.find(')') == line.size()-1){
            cexp = line.substr(1, line.size()-2); //expression in the brackets
            if ( existed("(", cexp) || existed("+", cexp) ||
                    existed("-", cexp) || existed("*", cexp) || existed("/", cexp)){
                    buildTree(cexp, node);
            }
            else{
                node->expr1->type = "factor";
                node->expr1->data = cexp;
            }
        }
        else{
            for (int i=0; i < static_cast<int>(line.size()); i++){
                if (line[i] == '(' || line[i] == ')') brackets.push(line[i]);
                else if(line[i] == '+' || line[i] == '-' ){
                    if (brackets.empty()||brackets.top()==')'){
                    lexp = line.substr(0, i);                    
                    rexp = line.substr(i+1);
                    node->symbol = line[i];
                    node->expr1 = new Node;
                    node->expr2 = new Node;
                    node->expr1->data = lexp;
                    if ( existed("(", lexp) || existed("+", lexp) ||
                        existed("-", lexp) || existed("*", lexp) || existed("/", lexp)){
                        node->expr1->type = "expression";
                        buildTree(lexp, node->expr1);
                    }
                    else node->expr1->type = "factor";
                    node->expr2->data = rexp;
                    if ( existed("(", rexp) || existed("+", rexp) ||
                        existed("-", rexp) || existed("*", rexp) || existed("/", rexp)){
                        node->expr2->type = "expression";
                        buildTree(rexp, node->expr2);
                     }
                     else node->expr2->type = "factor";
                     flag = false;
                     break;
                }
                    else flag = true;
            }
                else flag = true;
            }
            if (flag == true && (existed("*", line) || existed("/", line))){
                for (int i=0; i < static_cast<int>(line.size()); i++){
                    if (line[i] == '(' || line[i] == ')') brackets.push(line[i]);
                    else if (existed("*", line) || existed("/", line)){
                        if(line[i] == '*' || line[i] == '/' ){
                            if (brackets.empty()||brackets.top()==')'){
                                lexp = line.substr(0, i);
                                rexp = line.substr(i+1);
                                node->symbol = line[i];
                                node->expr1 = new Node;
                                node->expr2 = new Node;
                                node->expr1->data = lexp;
                                if ( existed("(", lexp) || existed("+", lexp) ||
                                        existed("-", lexp) || existed("*", lexp) || existed("/", lexp)){
                                        node->expr1->type = "expression";
                                        buildTree(lexp, node->expr1);
                                }
                                else node->expr1->type = "factor";
                                node->expr2->data = rexp;
                                if ( existed("(", rexp) || existed("+", rexp) ||
                                        existed("-", rexp) || existed("*", rexp) || existed("/", rexp)){
                                        node->expr2->type = "expression";
                                        buildTree(rexp, node->expr2);
                                }
                                else node->expr2->type = "factor";
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    else if (existed("+", line) || existed("-", line)){
        if (line.find("+") < line.find("-")){
            symbolCheck(line, node, "+");
        }
        else symbolCheck(line, node, "-");
    }
    else if (existed("*", line) || existed("/", line)){
        if (line.find("*") < line.find("/")){
            symbolCheck(line, node, "*");
        }
        else symbolCheck(line, node, "/");
    }
}


void compiler(string str){
    lexer(str);
    translation(roots, str);
    variables.clear();
    variables.shrink_to_fit();
    roots.clear();
    roots.shrink_to_fit();
}
