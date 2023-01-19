#ifndef INSTMEMORY_H
#define INSTMEMORY_H

#include <vector>
#include <QStringList>

class instMemory
{
public:
    instMemory();
    ~instMemory();

    void push_back(int index);
    void setInst(QStringList list);
    int getSize() const;
    int getLineNum(int index) const;
    bool isEmpty();
    const QString getInst(int index) const;

    std::vector<int> lineNum;
    QStringList inst;
};



#endif // INSTMEMORY_H
