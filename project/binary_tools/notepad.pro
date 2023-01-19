QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Notepad
TEMPLATE = app

RESOURCES+= \
    notepad.qrc \
    notepad.qrc \
    notepad.qrc

HEADERS+=\
    CodeGen.h \
    LoaderAndSimulator.h \
    assembling.h \
    bit.h \
    compiler.h \
config.h \
    debugdialog.h \
    debugger.h \
    exe_instruction.h \
    iType.h \
    instmemory.h \
    jType.h \
    linker.h \
    mainwindow.h \
    notepad.h \
    objassM.h \
    rType.h \
    register.h \
    searchdialog.h \
    perferencedialog.h \
    stringProcessing.h

SOURCES+=\
    CodeGen.cpp \
    LoaderAndSimulator.cpp \
    assmToimplement.cpp \
    bit.cpp \
    compiler.cpp \
config.cpp \
    debugdialog.cpp \
    debugger.cpp \
    exe_instruction.cpp \
    formatOut.cpp \
    instmemory.cpp \
    mainwindow.cpp \
    notepad.cpp \
    main.cpp \
    register.cpp \
    searchdialog.cpp \
    perferencedialog.cpp

FORMS += \
    debugdialog.ui \
    gotolinedialog.ui \
    searchdialog.ui \
    perferencedialog.ui

DISTFILES += \
    notepad.pro.user \
    notepad.pro.user.2.5pre1 \
    notepad.pro.user.41b2a74 \
    notepad.pro.user.788b1e2.4.8-pre1 \
    notepad.pro.user.af9ebe1 \
    notepad.pro.user.dcfaa78 \
    notepad.pro.user.ed75d47 \
    notepad.pro.user.f2b5e94 \
    register.cpp - 快捷方式.lnk \
    register.h - 快捷方式.lnk
