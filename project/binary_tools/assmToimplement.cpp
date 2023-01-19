#include <cstdlib>
#include <fstream>
#include <iostream>
#include "assembling.h"
#include "objassM.h"
#include "linker.h"
#include "LoaderAndSimulator.h"

using namespace std;

objInfo assember(string ifile_n){
    // the part of assembling.
    ifstream ifile;

    objInfo objif;
    ifile.open(ifile_n);
    recordSymbol(ifile, objif);
    ifile.close();
    ifile.open(ifile_n);
    assembling(ifile, objif);

    return objif;
}

int linking(vector<string> filevec){
    linker * mylinker = new linker;
    for (int i = 0; i < (int) filevec.size(); i++) {
        objInfo ojbif = assember(filevec[i]);
        mylinker->linkNewFile(& ojbif);
    }
    string ofile_n = "objlinker.txt";
    ofstream ofile(ofile_n);
    formatOutput(ofile, *(mylinker->objifpt));
    return 0;
}

int assmToimplement(vector<string> filevec){
    linking(filevec);

    cout << dec;
    loader();
    return 0;
}
