/*
 * File: FindAreaCode.cpp
 * ----------------------
 * This program looks up a numeric area codes for the United States
 * and Canada.  The program works in both directions.  If the user
 * enters a number, the program prints out the state or province to
 * which that code is assigned.  If the user enters a name, it prints
 * out all the area codes assigned to that name.
 */

#include <iostream>
#include <fstream>
#include <string>
#include "error.h"
#include "map.h"
#include "simpio.h"
#include "strlib.h"
#include<console.h>

using namespace std;

/* Function prototypes */

void readCodeFile(string filename, Map<int,string> & map);

/* Main program */

int main() {
   Map<int,string> areaCodeToState;
   readCodeFile("AreaCodes.txt", areaCodeToState);
   while (true){
       string line;
       bool condition = false;
       cout <<"Enter area code or state name: ";
       getline(cin,line);
       //if the user enter nothing, the program will exit
       if (line == ""){
           break;
        }
       //if the user enter area code
       if ((stringIsInteger(line))&&areaCodeToState.containsKey(stringToInteger(line))){
           cout<<areaCodeToState.get(stringToInteger(line))<<endl;
           condition = true;
       }
       //if the user enter state name
       for (int i:areaCodeToState){
           if(areaCodeToState[i]==line){
           cout<<i<<endl;
           condition = true;
           }
       }
       //if the user enter invalid input
       if (condition==false){
           cout<<"your input is invalid, please input again"<<endl;
       }
   }

   return 0;
}

/*
 * Function: readCodeFile
 * Usage: readCodeFile(filename, map);
 * -----------------------------------
 * Reads a data file representing area codes and locations into the map,
 * which must be declared by the client.  Each line must consist of the
 * area code, a hyphen, and the name of the state/province.
 */

void readCodeFile(string filename, Map<int,string> & map){
    ifstream infile;
    string line;
    infile.open(filename.c_str());     
    while (getline(infile,line)){
        if ((line.length()<4)||(line[3]!='-')){
            continue;
        }
        int sign = 0;
        sign = int(line.find("-"));
        int areaCode = 0;
        areaCode = stringToInteger(line.substr(0,size_t(sign)));
        string location;
        location = line.substr(size_t(sign)+1);
        map.put(areaCode,location);
    }
    infile.close();
}


