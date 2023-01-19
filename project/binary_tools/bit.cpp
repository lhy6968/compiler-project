#include "bit.h"

Bit::Bit()
{
    initBitToHexTable();
    initHexToBitTable();
}
const QString Bit::_16To2(const QString& x) {
    QString res;
    for(auto s : x) {
        res += hexToBitTable[s];
    }
    return res;
}

const QChar Bit::_1bitToQChar_2(int x) {
    return x == 1 ? '1' : '0';
}

const QString Bit::_10ToString_n(unsigned int x, int n) {
    QString res;
    int i = 0;

    while(x > 0) { //mod2
        res = _1bitToQChar_2(x%2) + res;
        x /= 2;
        i++;
    }

    for(int j = n-i; j > 0; j--) {
        res = '0' + res;
    }
    if(res.size() > n) { //truncate
        res = res.mid(res.size() - n);
    }
    return res;
}

int Bit::_32bitToInt(const QString& s) {
    int res = 0;
    for(int i = 0; i < 32; i= i+4) {
        res += bitToHexTable[(s.mid(i, 4))] * (10^(7-i));
    }
    return res;
}

void Bit::initHexToBitTable() {
    hexToBitTable["0"] = "0000";
    hexToBitTable["1"] = "0001";
    hexToBitTable["2"] = "0010";
    hexToBitTable["3"] = "0011";
    hexToBitTable["4"] = "0100";
    hexToBitTable["5"] = "0101";
    hexToBitTable["6"] = "0110";
    hexToBitTable["7"] = "0111";
    hexToBitTable["8"] = "1000";
    hexToBitTable["9"] = "1001";
    hexToBitTable["A"] = "1010";
    hexToBitTable["B"] = "1011";
    hexToBitTable["C"] = "1100";
    hexToBitTable["D"] = "1101";
    hexToBitTable["E"] = "1110";
    hexToBitTable["F"] = "1111";
    hexToBitTable["a"] = "1010";
    hexToBitTable["b"] = "1011";
    hexToBitTable["c"] = "1100";
    hexToBitTable["d"] = "1101";
    hexToBitTable["e"] = "1110";
    hexToBitTable["f"] = "1111";
}

void Bit::initBitToHexTable() {
    bitToHexTable["0000"] = 0;
    bitToHexTable["0001"] = 1;
    bitToHexTable["0010"] = 2;
    bitToHexTable["0011"] = 3;
    bitToHexTable["0100"] = 4;
    bitToHexTable["0101"] = 5;
    bitToHexTable["0110"] = 6;
    bitToHexTable["0111"] = 7;
    bitToHexTable["1000"] = 8;
    bitToHexTable["1001"] = 9;
    bitToHexTable["1010"] = 10;
    bitToHexTable["1011"] = 11;
    bitToHexTable["1100"] = 12;
    bitToHexTable["1101"] = 13;
    bitToHexTable["1110"] = 14;
    bitToHexTable["1111"] = 15;
}
