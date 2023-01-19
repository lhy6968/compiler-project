#include<QDebug>
#include <QFile>
#include <QFileInfo>
#include<QApplication>
#include"config.h"

Config::Config()
{
    iniFile = QApplication::applicationDirPath();
    iniFile += iniFile.endsWith('/') ? "settings.ini" : "/settings.ini";
    createIniFile(iniFile);

    QSettings settings(iniFile, QSettings::IniFormat);

    settings.beginGroup("General");
    recentFiles = settings.value("recentFiles").toStringList();
    maxRecentFiles = settings.value("maxRecentFiles", 10).toInt();
    showReadme = settings.value("showReadme", false).toBool();
    mainWindowsGeometry = settings.value("mainWindowGeometry").toByteArray();
    mainWindowState = settings.value("mainWindoState").toByteArray();
    settings.endGroup();

    settings.beginGroup("Editor");
    fontFamily = settings.value("fontFamily", "CourierNew").toString();
    fontSize = settings.value("fontSize", 12).toInt();
    fontStyle = settings.value("fontStyle", "Normal").toString();
    showLineNumber = settings.value("showLineNumber", true).toBool();
    tabIndents = settings.value("tabIndents").toBool(); //false
    autoIndent = settings.value("autoIndent").toBool();
    backUnindent = settings.value("backUnindent").toBool();
    spaceTabs = settings.value("spaceTabs", true).toBool();
    indentSize = settings.value("indentSize", 4).toInt();
    tabSize = settings.value("tabSize", 4).toInt();
    whitespaces = settings.value("whitespaces").toBool();
    settings.endGroup(); //Editor

    settings.beginGroup("Search&Replace");
    findHistory = settings.value("findHistory").toStringList();
    replaceHistory = settings.value("replaceHistory").toStringList();
    maxHistory = settings.value("maxHistory").toInt();
    matchCase = settings.value("matchCase").toBool();
    regExp = settings.value("regExp").toBool();
    settings.endGroup();

}

Config::~Config()
{
    iniFile = QApplication::applicationDirPath();
    iniFile += iniFile.endsWith('/') ? "settings.ini" : "/settings.ini";
    createIniFile(iniFile);
    QSettings settings(iniFile, QSettings::IniFormat);

    settings.beginGroup("General");
    settings.setValue("recentFiles", recentFiles);
    settings.setValue("maxRecentFile", maxRecentFiles);
    settings.setValue("showReadme", showReadme);
    settings.setValue("mainWindowGeometry", mainWindowsGeometry);
    settings.setValue("mainWindoState", mainWindowState);
    settings.endGroup();

    settings.beginGroup("Editor");
    settings.setValue("fontFamily", fontFamily);
    settings.setValue("fontSize", fontSize);
    settings.setValue("fontStyle", fontStyle);
    settings.setValue("showLineNumber", showLineNumber);
    settings.setValue("tabIndents", tabIndents);
    settings.setValue("autoIndent", autoIndent);
    settings.setValue("backUnindent", backUnindent);
    settings.setValue("spaceTabs", spaceTabs);
    settings.setValue("indentSize", indentSize);
    settings.setValue("tabSize", tabSize);
    settings.setValue("whitespaces", whitespaces);
    settings.endGroup(); //Editor

    settings.beginGroup("Search&Replace");
    findHistory.sort();
    replaceHistory.sort();
    settings.setValue("findHistory", findHistory);
    settings.setValue("maxHistory", maxHistory);
    settings.setValue("replaceHistory", replaceHistory);
    settings.setValue("matchCase", matchCase);
    settings.setValue("regExp", regExp);
    settings.endGroup();
}

// reconfiguration
void Config::reconfig(int receiver)
{
    emit reread(receiver);
}

// create settings.ini file
void Config::createIniFile(QString iniFile)
{
    if (QFile::exists(iniFile))
        return;
    else
    {
        QFile file(iniFile);
        file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
        file.close();
    }
}
