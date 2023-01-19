#ifndef BIT_H
#define BIT_H


#include <QString>
#include <map>

class Bit
{
public:
    Bit();

    void initHexToBitTable();
    void initBitToHexTable();
    std::map<QString, QString> hexToBitTable;
    std::map<QString, int> bitToHexTable;

    int _32bitToInt(const QString& s);
    const QString _16To2(const QString& x);
    const QChar _1bitToQChar_2(int x);


    const QString _10ToString_n(unsigned int x, int n=5);
};

#endif // BIT_H
