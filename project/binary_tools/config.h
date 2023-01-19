#ifndef CONFIG_H
#define CONFIG_H
#include <QObject>
#include <QStringList>
#include <QString>
#include <QByteArray>
#include <QSettings>

struct Config: public QObject
{
    Q_OBJECT

public:
    enum Receiver
    {
        Init = 1, //0000001
        Editor = 2, //0000010
        Highlighter = 4 //0000100
    };

signals:
    void reread(int);
private:
    void createIniFile(QString iniFile);    //create settings.ini file

public slots:
    void reconfig(int receiver);    //reconfiguration

public:
    Config();
    ~Config();

    QString iniFile;    //settings.ini file path
    //General
    QByteArray mainWindowsGeometry;
    QByteArray mainWindowState;

    int maxRecentFiles;
    QStringList recentFiles;
    bool showReadme;

    //Editor
    QString fontFamily; //font set
    int fontSize;
    QString fontStyle;
    bool showLineNumber;

    bool tabIndents; //tab indent
    bool autoIndent; //auto indent
    bool backUnindent; //unindent
    bool spaceTabs; //show space or Tab

    int indentSize;
    int tabSize;

    bool whitespaces; //white space to replace Tab

    //Search
    int maxHistory;
    QStringList findHistory; //find history record
    QStringList replaceHistory; //replace history record

    bool matchCase;
    bool regExp;
};

#endif//CONFIG_H
