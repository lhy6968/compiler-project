#include <QApplication>
#include "mainwindow.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Config config;
    MainWindow mainWin(&config);
    mainWin.show();
    return app.exec();
}
