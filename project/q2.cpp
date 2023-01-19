/*
 * File: q2.cpp
 * --------------------------------
 * This program achieve the requirement in q2
 * It ask the user to input a longer DNA strand
 * and a shorter one and then the program will
 * show the matching positions corresponding to these two DNA
 * strand in the longer DNA strand
 */

#include "q2.h"
#include <string>
#include <iostream>
using namespace  std;

// Constants and Types
const string HEADER = "CSC3002 Assignment1 Question2: Find DNA Match";
const string PROMPT_DNA = "Enter a longer DNA strand (e.g. TAACGGTACGTC): ";
const string PROMPT_SNT = "Enter a shorter one (e.g. TGC):";
const string ENDPRO = "End of Question2";

/*
 * Function q2
 * Usage q2();
 * --------------------------------------------
 * This function show some information of q2 and then ask the user to input a longer DNA strand
 * and a shorter one. The function will show the matching positions corresponding to these two DNA strand
 * in the longer DNA strand and finally exit the program
 */
void q2(){
    //variables and types
    string DNA_strand;
    string snippet;
    bool condition_1 = true;
    bool condition_2 = true;
    int start_index = 0;

    cout << HEADER << endl;

    /*
     * the input must be a DNA strand
     * the program will judge and let the user enter again
     * if the input is not right
     */
    while (condition_1){
        cout<<PROMPT_DNA;
        cin >> DNA_strand;
        for (int i = 0;i<int(DNA_strand.length());i=i+1){
            if (DNA_strand[size_t(i)]=='A'||DNA_strand[size_t(i)]=='T'||DNA_strand[size_t(i)]=='C'||DNA_strand[size_t(i)]=='G'){
                condition_1 = false;
                continue;
            }
            else{
                cout<<"Your input is not a DNA strand, please enter again"<<endl;
                condition_1 = true;
                break;
            }
        }
    }

    /*
     * for the second DNA_strand(the shorter one),
     * it should not only be a DNA strand but also be shorter than the first one
     * the program will judge and let the user enter again
     * if the input is not right
     */
    while (condition_2){
        cout << PROMPT_SNT;
        cin >> snippet;
        if (snippet.length()>DNA_strand.length()){
            cout<<"The shorter one must be shorter than the longer one,please enter again"<<endl;
            continue;
        }
        for (int i = 0;i<int(snippet.length());i=i+1){
            if (snippet[size_t(i)]=='A'||snippet[size_t(i)]=='T'||snippet[size_t(i)]=='C'||snippet[size_t(i)]=='G'){
                condition_2 = false;
                continue;
            }
            else{
                cout<<"Your input is not a DNA strand,please enter again"<<endl;
                condition_2 = true;
                break;
            }
        }
    }

    findDNAMatch(DNA_strand,snippet,start_index);
    cout << ENDPRO << endl;
}

/*
 *  Function: findDNAMatch
 * Usage: int start = findDNAMatch(s1,s2,start);
 * -----------------------------------------------------------
 * This function will get the input two string (the longer DNA strand and the shorter one)
 * and the index integer of beginning. It will show all the matching positions to the user and
 * finally return the index integer of end.
 */
int findDNAMatch(string s1, string s2, int start) {
    for (int i = 0; i< int(s2.length());i = i+1){
        if (s2[size_t(i)]== 'A'){
           s2.replace(size_t(i),1,"T");
        }
        else if (s2[size_t(i)]=='T'){
            s2.replace(size_t(i),1,"A");
        }
        else if (s2[size_t(i)]=='C'){
            s2.replace(size_t(i),1,"G");
        }
        else if (s2[size_t(i)]=='G'){
            s2.replace(size_t(i),1,"C");
        }
    }

    /* if the two DNA strand can not match,
     * the program will tell the user
     */
    if (int(s1.find(s2))==-1){
        start = int(s1.find(s2));
        cout<< "sorry,these two DNA strand can not match at any positions" << endl;
    }
    else{
        cout << "The matching positions should be: ";
        int time = 0;
        while (int(s1.find(s2))!=-1){
            if (time == 0){
                start = start + int(s1.find(s2));
                time = time + 1;
            }
            else {
                start = start + int(s1.find(s2));
            }
            s1 = s1.substr(s1.find(s2)+1);
            if (int(s1.find(s2))!=-1){
                cout << start << " , ";
                start = start + 1;
            }
            else{
                cout << start << endl;
            }
        }
    }
    return start;
}
