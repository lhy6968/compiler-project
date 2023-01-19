#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "notepad.h"
#include "searchdialog.h"
#include "debugdialog.h"
#include "perferencedialog.h"
#include "config.h"
#include <vector>
#include <string>
#include "debugger.h"
#include "exe_instruction.h"


QT_BEGIN_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QTabWidget)
QT_FORWARD_DECLARE_CLASS (QMenuBar)
QT_FORWARD_DECLARE_CLASS (QToolBar)
QT_FORWARD_DECLARE_CLASS (QFont)
QT_FORWARD_DECLARE_CLASS (QComboBox)
QT_FORWARD_DECLARE_CLASS (QFontComboBox)
QT_FORWARD_DECLARE_CLASS (QActionGroup)
QT_FORWARD_DECLARE_CLASS (QTextCharFormat)
QT_FORWARD_DECLARE_CLASS (QPrinter)
QT_END_NAMESPACE

#define EDITOR   static_cast<NotePad *>(tabWidget->currentWidget())

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(Config *, QWidget * = 0);
    ~MainWindow();

signals:

public slots:

public slots:
    void currentChanged(int index);
    void modificationChanged(bool changed);
    void selectionChanged();    //file selection change
    void blockCountChanged(int blockCount);   //block count change
    void openFile();
    void openFile(QString FileName);
    void newFile();
    bool fileSaveAs(int index);
    bool fileSave(int index);
    bool fileSaveAs();  //save current file as
    bool fileSave();    //save current file
    bool runAll();
    bool run(int index, std::vector<std::string> & file_vec);   //run file
    bool fileSaveAll(); //save all

    void fileClose(int index);
    void fileClose();   //close file
    void fileCloseAll();    //close all files

    void debug();
    void search();

    void textColor(); //set text color
    void textFont(); //set text font
    void textFontFamily(const QString& font);
    void textSize(const QString &size); // set text size
    void textStyle(int index);
    void textStyle();    // set text style
    void updateTextStyleActs(QString style);

    void nextWindow();
    void previousWindow();
    void currentAllWindow();
    void setCurrentWindow(QAction *a);
    void openRecentFile();
    void updateRecentFiles();

    void setupDebugActions();
public:
    Config *config; //editor configuration
    QTabWidget *tabWidget;  // Tab
    SearchDialog *searchDialog; // Find/replace block
    GoToLineDialog * gotoLineDialog;   // debugger
    PerferenceDialog *perferenceDialog; // first choice
    int newNumber;  //new file number
    QStringList openedFiles;    //opened file
    QList<QAction *> recentFileActs; //recent opened file
    QActionGroup *openedFilesGrp; // window Action Group

    QMenuBar *menuBar;  //menu
    QToolBar *topToolBar;   //first tool bar
    QToolBar *bottomToolBar;    //second tool bar

    QComboBox *comboStyle;  // font style combo box
    QFontComboBox *comboFont;   // font combo box
    QComboBox *comboSize;   //font size combo box

    QMenu *fileMenu;    //file menu
    QAction *openAct;   // open file
    QAction *newAct;    // new file
    QAction *saveAct;   // save
    QAction *saveAsAct;     // save as
    QAction *saveAllAct;    // save all

    QAction *closeAct;  // close file
    QAction *closeAllAct;   // close all file
    QAction *exitAct;   // exit

    QMenu *editMenu;    // edit menu
    QAction *copyAct;   // copy
    QAction *cutAct;    // cut
    QAction *pasteAct;  // paste
    QAction *undoAct;   // undo
    QAction *redoAct;   // redo
    QAction *selectAllAct;  // select all
    QAction *upperCaseAct;
    QAction *lowerCaseAct;
    QAction *findAct;   // search and replace
    QAction *gotoLineAct;   // go to line
    QAction *gotoLineAct2;
    QAction *preferenceAct; // first choice

    QMenu *formatMenu;  // file format menu
    QAction *colorAct;    // color
    QMenu *fontStyleMenu;   // font style memu
    QAction *boldAct;   // bold
    QAction *italicAct; // italic
    QAction *underlineAct;  // underline
    QAction *fontAct;   // font choice

    QMenu *windowMenu;  // window menu
    QAction *nextAct;   // next window
    QAction *previousAct;   // previous window
    QMenu *recentlyFilesMenu; // recent file menu
    QMenu *currentAllMenu;  // current all menu

    QMenu *helpMenu;    // help menu
    QAction * actionStart;
    QAction * actionSingle_step;
    QAction * actionRun;


    void init();    //initial
    void closeEvent(QCloseEvent *);
    void setupFileMenu();
    void setupFileActions();
    void setupEditMenu();
    void setupEditActions();
    void setupFormatMenu();
    void setupFormatActions();
    void setupWindowMenu();
    void fillRecentFileActs();
    void setupWindowActions();
    void debugMenu();   // For debugger menu
    void setupHelpActions();

    void newTab(const QString& fileName, QFile& file);  // create new tab (to open file)
    bool maybeSave(int index); // check whether to save
    void closeDuplicate(int index); // close the file which is opened multiple times
    void updateActions();   // update the states of all actions
    void refreshActions();  // refresh the states of all actions
    void updateRecentFilesList();
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format); // set text format
    void colorChanged(const QColor &col);
    void fontChanged(const QFont &font);
    void updateComboStyle();
    void currentCharFormatChanged(const QTextCharFormat &format);

};

#endif // MAINWINDOW_H
