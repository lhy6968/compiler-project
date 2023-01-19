#include "mainwindow.h"
#include <QPrintDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QPrintEngine>
#include <QTextStream>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QTabWidget>
#include <QTextDocumentWriter>
#include <QMessageBox>
#include <QFileDialog>
#include <QKeySequence>
#include <QMenuBar>
#include <QToolBar>
#include <QFont>
#include <QClipboard>
#include <QComboBox>
#include <QFontComboBox>
#include <QFontDatabase>
#include <QActionGroup>
#include <QTextCharFormat>
#include <QMimeData>
#include <iostream>
#include <fstream>
#include <string>
#include "compiler.h"
#include "assmToimplement.cpp"
#include <qthread.h>
#include <QTextEdit>
using namespace std;

MainWindow::MainWindow(Config *config, QWidget *parent) :
    QMainWindow(parent), config(config)
{
    init();
    setupFileMenu();
    setupEditMenu();
    setupFormatMenu();
    setupWindowMenu();
    debugMenu();
    currentChanged(-1);
    currentChanged(0);
    setupEditActions();
    setCentralWidget(tabWidget);
    restoreGeometry(config->mainWindowsGeometry);
    restoreState(config->mainWindowState);
}

// initialize
void MainWindow::init()
{
    menuBar = new QMenuBar(this);
    topToolBar = new QToolBar(this);
    topToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    bottomToolBar = new QToolBar(this);
    bottomToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    setMenuBar(menuBar);
    addToolBar(topToolBar);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(bottomToolBar);

    comboFont = new QFontComboBox(bottomToolBar);
    comboFont->setCurrentFont(QFont(config->fontFamily));
    bottomToolBar->addWidget(comboFont);
    comboStyle = new QComboBox(bottomToolBar);
    comboStyle->addItem("Normal");
    comboStyle->addItem("Bold");
    comboStyle->addItem("Italic");
    comboStyle->addItem("Bold Italic");
    comboStyle->setCurrentIndex(comboStyle->findText(config->fontStyle));
    bottomToolBar->addWidget(comboStyle);
    comboSize = new QComboBox(bottomToolBar);
    comboSize->setEditable(true);

    QFontDatabase db;
    foreach(int size, db.standardSizes())
        comboSize->addItem(QString::number(size));
    comboSize->setCurrentIndex(
                comboSize->findText(tr("%1").arg(config->fontSize)));
    bottomToolBar->addWidget(comboSize);

    tabWidget = new QTabWidget(this);
    tabWidget->setMovable(true);
    tabWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    tabWidget->setTabsClosable(true);
    connect(tabWidget,SIGNAL(currentChanged(int)),this,SLOT(currentChanged(int)));
    connect(tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(fileClose(int)));

    searchDialog = new SearchDialog(config);
    searchDialog->setVisible(false);

    gotoLineDialog = new GoToLineDialog;
    gotoLineDialog->setVisible(false);

    perferenceDialog = new PerferenceDialog(config, this);
    perferenceDialog->setVisible(false);
}

// close Event
void MainWindow::closeEvent(QCloseEvent *event)
{
    for (int i = 0; i < tabWidget->count(); i++)
    {
        tabWidget->setCurrentIndex(i);
        if (!maybeSave(i))
        {
            event->ignore();
            return;
        }
    }
    event->accept();
}

// modification occurs
void MainWindow::modificationChanged(bool changed)
{
    QString str = tabWidget->tabText(tabWidget->currentIndex());
    if (str[str.length() - 1] == '*')
    {
        if (!changed)
            str.resize(str.length() - 1);
    }
    else if (changed)
    {
        str += '*';
    }
    tabWidget->setTabText(tabWidget->currentIndex(), str);
    refreshActions();
    setupEditActions();
}

// selection change occurs
void MainWindow::selectionChanged()
{
    upperCaseAct->setEnabled(EDITOR->textCursor().hasSelection());
    lowerCaseAct->setEnabled(EDITOR->textCursor().hasSelection());
    QTextCharFormat fmt=EDITOR->currentCharFormat();
    currentCharFormatChanged(fmt);
}

// block occurs
void MainWindow::blockCountChanged(int blockCount)
{
    gotoLineDialog->setMaxLineNumber(blockCount);
}

// Tab change accurs
void MainWindow::currentChanged(int index)
{
    if (index == -1)
    {
        newNumber = 0;
        newFile();
        updateTextStyleActs(config->fontStyle);
        return;
    }
    updateActions();
    setWindowIcon(QIcon(tr(":images/notepad.png")));
    setWindowTitle(tr("NotePad(%1)").arg(openedFiles.at(index)));
}

// create new tab (to open file)
void MainWindow::newTab(const QString& fileName, QFile& file)
{
    openedFiles << fileName;
    NotePad *notePad = new NotePad(config);
    tabWidget->addTab(notePad, QFileInfo(fileName).fileName());
    QByteArray data = file.readAll();
    notePad->setPlainText(QString::fromLocal8Bit(data));
    tabWidget->setCurrentWidget(notePad);
}


// check whether there is a need to save
bool MainWindow::maybeSave(int index)
{
    NotePad *notePad = static_cast<NotePad*>(tabWidget->widget(index));
    QString fileName = openedFiles.at(index);
    if (!notePad->document()->isModified())
        return true;
    if (fileName.startsWith(QLatin1String(":/")))
        return true;
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("NotePad"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
        return fileSave(index);
    else if (ret == QMessageBox::Cancel)
        return false;
    return true;
}

// update the recent file list
void MainWindow::updateRecentFilesList()
{
    int index = tabWidget->currentIndex();
    QString fileName = openedFiles.at(index);
    config->recentFiles.removeAll(fileName);
    config->recentFiles.prepend(fileName);
    if (config->recentFiles.size() > config->maxRecentFiles)
        config->recentFiles.removeLast();
}

// refresh the state of actions (for subfunction)
void MainWindow::refreshActions()
{
    saveAct->setEnabled(EDITOR->document()->isModified());
    copyAct->setEnabled(EDITOR->textCursor().hasSelection());
    cutAct->setEnabled(EDITOR->textCursor().hasSelection());
    undoAct->setEnabled(EDITOR->document()->isUndoAvailable());
    redoAct->setEnabled(EDITOR->document()->isRedoAvailable());
#ifndef QT_NO_CLIPBOARD
    if (const QMimeData *md = QApplication::clipboard()->mimeData())
        pasteAct->setEnabled(md->hasText());
#endif
    gotoLineDialog->setMaxLineNumber(EDITOR->blockCount());
    nextAct->setEnabled(tabWidget->currentIndex()<tabWidget->count()-1);
    previousAct->setEnabled(tabWidget->currentIndex()>=1);
}

// update the states of actions
void MainWindow::updateActions()
{
    connect(EDITOR, SIGNAL(modificationChanged(bool)), SLOT(modificationChanged(bool)), Qt::UniqueConnection);
    connect(EDITOR,SIGNAL(selectionChanged()),SLOT(selectionChanged()));
    connect(EDITOR,SIGNAL(blockCountChanged(int)),SLOT(blockCountChanged(int)));
    refreshActions();
    updateRecentFilesList();
}


void MainWindow::closeDuplicate(int index)
{
    QString fileName = openedFiles.at(index);
    for (int i = 0; i < openedFiles.count(); i++)
    {
        if (openedFiles.at(i) == fileName && i != index)
        {
            openedFiles.removeAt(i);
            tabWidget->removeTab(i);
        }
    }
    int currIndex = openedFiles.indexOf(fileName);
    tabWidget->setCurrentIndex(currIndex);
    setWindowTitle(tr("NotePad(%1)").arg(fileName));
}


void MainWindow::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = EDITOR->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    EDITOR->mergeCurrentCharFormat(format);
}


void MainWindow::openFile(QString fileName)
{
    int index = openedFiles.indexOf(fileName);
    if (index != -1)
        tabWidget->setCurrentIndex(index);
    else
    {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly))
            newTab(fileName, file);
    }
}


void MainWindow::openFile()
{
    QStringList files;
    files = QFileDialog::getOpenFileNames(this, tr("Open files..."), QString(),
                                          tr("All Files(*);;Text Files(*.txt)"));
    QString fileName;
    if (files.count())
    {
        for (int i = 0; i < files.count(); i++)
        {
            fileName = files.at(i);
            if (QFile::exists(fileName))
            {
                QFile file(fileName);
                if (file.open(QFile::ReadOnly))
                {
                    if (openedFiles.contains(fileName))
                        continue;
                    newTab(fileName, file);
                }
            }
        }
    }
}


void MainWindow::newFile()
{
    QString fileName = tr("New %1").arg(++newNumber);
    openedFiles << fileName;
    tabWidget->setCurrentIndex(
                tabWidget->addTab(new NotePad(config), fileName));
}


bool MainWindow::fileSaveAs(int index)
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(),
                                              tr("Plain text Files(*.txt);;All Files (*)"));
    if (fn.isEmpty())
        return false;
    if (!fn.endsWith(".txt", Qt::CaseInsensitive))
        fn += ".txt";

    openedFiles.replace(index, fn);
    tabWidget->setTabText(index, QFileInfo(fn).fileName());
    return fileSave(index);
}


bool MainWindow::fileSaveAs()
{
    return fileSaveAs(tabWidget->currentIndex());
}


bool MainWindow::fileSave(int index)
{
    NotePad *notePad = static_cast<NotePad*>(tabWidget->widget(index));
    QString fileName = openedFiles.at(index);
    if (!fileName.contains("/") && !fileName.contains("\\"))
        return fileSaveAs(index);
    QTextDocumentWriter writer(fileName);
    string str = fileName.toStdString();

    bool success = writer.write(notePad->document());
    if (success)
    {
        notePad->document()->setModified(false);
        tabWidget->setCurrentWidget(notePad);
        setWindowTitle(tr("NotePad(%1)").arg(fileName));
    }
    closeDuplicate(index);
    return success;
}


bool MainWindow::fileSave()
{
    return fileSave(tabWidget->currentIndex());
}


bool MainWindow::runAll()
{
    vector<string> file_vec;
    bool success = true;
    for (int i = 0; i < tabWidget->count(); i++)
    {
        tabWidget->setCurrentIndex(i);
        success = run(i, file_vec);
    }
    assmToimplement(file_vec);
    return success;

}


bool MainWindow::run(int index, vector<string> & file_vec)
{
    NotePad *notePad = static_cast<NotePad*>(tabWidget->widget(index));
    QString fileName = openedFiles.at(index);
    if (!fileName.contains("/") && !fileName.contains("\\"))
        return fileSaveAs(index);
    QTextDocumentWriter writer(fileName);
    string str = fileName.toStdString();
    for (int i = 0;i<static_cast<int>(str.length());i++){
        if (str[size_t(i)]=='/'){
            str[size_t(i)] = '\\';
        }
    }
    cout << " The input file is --> " << str<<endl;
    bool success = writer.write(notePad->document());
    if (success)
    {
        notePad->document()->setModified(false);
        tabWidget->setCurrentWidget(notePad);
        setWindowTitle(tr("NotePad(%1)").arg(fileName));
    }

    closeDuplicate(index);

    string filename;
    compiler(str);
    filename = str.substr(0, str.find_last_of('.')) + ".asm";
    file_vec.push_back(filename);
    return success;
}


bool MainWindow::fileSaveAll()
{
    bool success = true;
    for (int i = 0; i < tabWidget->count(); i++)
    {
        tabWidget->setCurrentIndex(i);
        success = fileSave(i);
    }
    return success;
}


void MainWindow::fileClose(int index)
{
    if (maybeSave(index))
    {
        if (openedFiles.count() == 1)
        {
            newFile();
            openedFiles.removeAt(0);
            tabWidget->removeTab(0);
        }
        else
        {
            openedFiles.removeAt(index);
            tabWidget->removeTab(index);
        }
    }
}


void MainWindow::fileClose()
{
    fileClose(tabWidget->currentIndex());
}


void MainWindow::fileCloseAll()
{
    while (tabWidget->count() >= 1)
    {
        if (maybeSave(tabWidget->currentIndex()))
        {
            if (openedFiles.count() == 1)
            {
                newFile();
                openedFiles.removeAt(0);
                tabWidget->removeTab(0);
                break;
            }
            else
            {
                openedFiles.removeAt(tabWidget->currentIndex());
                tabWidget->removeTab(tabWidget->currentIndex());
            }
        }
        else
            break;
    }
}

// set up file menu
void MainWindow::setupFileMenu()
{
    fileMenu = new QMenu(tr("&File"), menuBar);

    //open file
    openAct = new QAction(QIcon(tr(":images/fileopen.png")), tr("&Open"), this);
    openAct->setShortcut(QKeySequence::Open);
    fileMenu->addAction(openAct);
    topToolBar->addAction(openAct);

    //new file
    newAct = new QAction(QIcon(tr(":images/filenew.png")), tr("&New"), this);
    newAct->setShortcut(QKeySequence::New);
    fileMenu->addAction(newAct);
    topToolBar->addAction(newAct);

    fileMenu->addSeparator();

    //save
    saveAct = new QAction(QIcon(tr(":images/filesave.png")), tr("&Save"), this);
    saveAct->setShortcut(QKeySequence::Save);
    fileMenu->addAction(saveAct);
    topToolBar->addAction(saveAct);

    //save as
    saveAsAct = new QAction(QIcon(tr(":images/filesaveas.png")),
                            tr("Save &As..."), this);
    saveAsAct->setShortcut(QKeySequence::SaveAs);
    fileMenu->addAction(saveAsAct);
    topToolBar->addAction(saveAsAct);

    //save all
    saveAllAct = new QAction(QIcon(tr(":images/saveall.png")), tr("Save All"),
                             this);
    saveAllAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    fileMenu->addAction(saveAllAct);
    topToolBar->addAction(saveAllAct);

    fileMenu->addSeparator();

    //close current
    closeAct = new QAction(QIcon(tr(":images/fileclose.png")), tr("Close"),
                           this);
    closeAct->setShortcut(QKeySequence::Close);
    fileMenu->addAction(closeAct);
    topToolBar->addAction(closeAct);

    //close all
    closeAllAct = new QAction(QIcon(tr(":images/closeall.png")),
                              tr("Close All"), this);
    closeAllAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_X);
    fileMenu->addAction(closeAllAct);
    topToolBar->addAction(closeAllAct);

    //exit
    exitAct = new QAction(QIcon(tr(":images/fileexit.png")), tr("Exit"), this);
    exitAct->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(exitAct);
    topToolBar->addAction(exitAct);

    topToolBar->addSeparator();

    menuBar->addMenu(fileMenu);
    setupFileActions();
}

// set up Actions in file menu
void MainWindow::setupFileActions()
{
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(fileSave()));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
    connect(saveAllAct, SIGNAL(triggered()), this, SLOT(fileSaveAll()));
#ifndef QT_NO_PRINTER

#endif
    connect(closeAct, SIGNAL(triggered()), this, SLOT(fileClose()));
    connect(closeAllAct, SIGNAL(triggered()), this, SLOT(fileCloseAll()));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
}

//debug
void MainWindow::debug()
{
    if (gotoLineDialog->isVisible())
        gotoLineDialog->activateWindow();
    else
        gotoLineDialog->show();

    string file_n;
    vector<string> file_vec;
    bool success = true;
    for (int i = 0; i < tabWidget->count(); i++)
    {
        tabWidget->setCurrentIndex(i);
        success = run(i, file_vec);
    }
    linking(file_vec);
    gotoLineDialog->regs->fresh();
    gotoLineDialog->finish = 0;
    gotoLineDialog->debugger->regs->fresh();
    gotoLineDialog->debugger->init();
    gotoLineDialog->display();
    connect(gotoLineDialog,SIGNAL(debug(int)),EDITOR,SLOT(debug(int)));
}

// set up edit menu
void MainWindow::setupEditMenu()
{
    editMenu = new QMenu(tr("&Edit"), menuBar);

    // copy
    copyAct = new QAction(QIcon(tr(":images/editcopy.png")), tr("&Copy"), this);
    copyAct->setShortcut(QKeySequence::Copy);
    editMenu->addAction(copyAct);
    topToolBar->addAction(copyAct);

    // cut
    cutAct = new QAction(QIcon(tr(":images/editcut.png")), tr("&Cut"), this);
    cutAct->setShortcut(QKeySequence::Cut);
    editMenu->addAction(cutAct);
    topToolBar->addAction(cutAct);

    // paste
    pasteAct = new QAction(QIcon(tr(":images/editpaste.png")), tr("&Paste"), this);
    pasteAct->setShortcut(QKeySequence::Paste);
    editMenu->addAction(pasteAct);
    topToolBar->addAction(pasteAct);

    editMenu->addSeparator();

    // undo
    undoAct = new QAction(QIcon(tr(":images/editundo.png")), tr("&Undo"), this);
    undoAct->setShortcut(QKeySequence::Undo);
    editMenu->addAction(undoAct);
    topToolBar->addAction(undoAct);

    // redo
    redoAct = new QAction(QIcon(tr(":images/editredo.png")), tr("&Redo"), this);
    redoAct->setShortcut(QKeySequence::Redo);
    editMenu->addAction(redoAct);
    topToolBar->addAction(redoAct);

    editMenu->addSeparator();

    // select all
    selectAllAct = new QAction(QIcon(tr(":images/editselectall.png")),
                               tr("&Select all"), this);
    selectAllAct->setShortcut(QKeySequence::SelectAll);
    editMenu->addAction(selectAllAct);
    topToolBar->addAction(selectAllAct);

    // to upper case
    upperCaseAct = new QAction(QIcon(tr(":images/uppercase.png")), tr("UpperCase"),
                               this);
    editMenu->addAction(upperCaseAct);
    topToolBar->addAction(upperCaseAct);

    // to lower case
    lowerCaseAct = new QAction(QIcon(tr(":images/lowercase.png")), tr("LowerCase"),
                               this);
    editMenu->addAction(lowerCaseAct);
    topToolBar->addAction(lowerCaseAct);

    editMenu->addSeparator();

    // go to line
    gotoLineAct = new QAction(tr("Goto Line..."), this);
    gotoLineAct->setShortcut(Qt::CTRL + Qt::Key_G);
    editMenu->addAction(gotoLineAct);

    // find and replace
    findAct = new QAction(QIcon(tr(":images/editfind.png")), tr("&Find/Replace"),
                          this);
    findAct->setShortcut(QKeySequence::Find);
    editMenu->addAction(findAct);
    topToolBar->addAction(findAct);

    // run
    gotoLineAct2 = new QAction(QIcon(tr(":1/run2.png")),tr("Run"), this);
    gotoLineAct2->setShortcut(Qt::CTRL + Qt::Key_R);
    editMenu->addAction(gotoLineAct2);
    topToolBar->addAction(gotoLineAct2);
    topToolBar->addSeparator();

    menuBar->addMenu(editMenu);
}

// set up edit menu actions
void MainWindow::setupEditActions()
{
    connect(copyAct, SIGNAL(triggered()), EDITOR,SLOT(copy()));
    connect(pasteAct,SIGNAL(triggered()),EDITOR,SLOT(paste()));
    connect(undoAct,SIGNAL(triggered()),EDITOR,SLOT(undo()));
    connect(redoAct,SIGNAL(triggered()),EDITOR,SLOT(redo()));
    connect(selectAllAct,SIGNAL(triggered()),EDITOR,SLOT(selectAll()));
    connect(upperCaseAct,SIGNAL(triggered()),EDITOR,SLOT(toUpperCase()));
    connect(lowerCaseAct,SIGNAL(triggered()),EDITOR,SLOT(toLowerCase()));

    connect(findAct,SIGNAL(triggered()),this,SLOT(search()));
    connect(gotoLineAct2,SIGNAL(triggered()),this,SLOT(runAll()));
}


void MainWindow::textColor()
{
    QColor col = QColorDialog::getColor(Qt::black, this);
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    colorChanged(col);
}


void MainWindow::textFontFamily(const QString& font)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(font);
    currentCharFormatChanged(fmt);
    mergeFormatOnWordOrSelection(fmt);
}


void MainWindow::textFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, EDITOR->font(),this);
    if (!ok)
        return;
    QTextCharFormat fmt;
    fmt.setFont(font);
    mergeFormatOnWordOrSelection(fmt);
    fontChanged(font);
}


void MainWindow::textSize(const QString &size)
{
    qreal pointSize = size.toFloat();
    if (size.toFloat() > 0)
    {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        comboSize->setCurrentIndex(comboSize->findText(size));
        mergeFormatOnWordOrSelection(fmt);
    }
}


void MainWindow::colorChanged(const QColor &col)
{
    QPixmap pix(16, 16);
    pix.fill(col);
    colorAct->setIcon(pix);
}


void MainWindow::fontChanged(const QFont &font)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(font).family()));
    comboSize->setCurrentIndex(
                comboSize->findText(QString::number(font.pointSize())));
    boldAct->setChecked(font.bold());
    italicAct->setChecked(font.italic());
    underlineAct->setChecked(font.underline());
    updateComboStyle();
}


void MainWindow::updateComboStyle()
{
    if (boldAct->isChecked() && italicAct->isChecked())
        comboStyle->setCurrentIndex(comboStyle->findText(tr("Bold Italic")));
    else if (boldAct->isChecked() && !italicAct->isChecked())
        comboStyle->setCurrentIndex(comboStyle->findText(tr("Bold")));
    else if (!boldAct->isChecked() && italicAct->isChecked())
        comboStyle->setCurrentIndex(comboStyle->findText(tr("Italic")));
    else
        comboStyle->setCurrentIndex(comboStyle->findText(tr("Normal")));
}


void MainWindow::textStyle()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(boldAct->isChecked() ? QFont::Bold : QFont::Normal);
    fmt.setFontItalic(italicAct->isChecked());
    fmt.setFontUnderline(underlineAct->isChecked());
    mergeFormatOnWordOrSelection(fmt);
    fontChanged(fmt.font());
}


void MainWindow::updateTextStyleActs(QString style)
{
    if (style == "Normal")
    {
        boldAct->setChecked(false);
        italicAct->setChecked(false);
    }
    else if (style == "Bold")
    {
        boldAct->setChecked(true);
        italicAct->setChecked(false);
    }
    else if (style == "Italic")
    {
        boldAct->setChecked(false);
        italicAct->setChecked(true);
    }
    else
    {
        boldAct->setChecked(true);
        italicAct->setChecked(true);
    }
}


void MainWindow::textStyle(int styleIndex)
{
    QTextCharFormat fmt;
    if (styleIndex == 0)
    {
        fmt.setFontItalic(false);
        fmt.setFontWeight(QFont::Normal);
    }
    else if (styleIndex == 1)
    {
        fmt.setFontItalic(false);
        fmt.setFontWeight(QFont::Bold);
    }
    else if (styleIndex == 2)
    {
        fmt.setFontItalic(true);
        fmt.setFontWeight(QFont::Normal);
    }
    else
    {
        fmt.setFontItalic(true);
        fmt.setFontWeight(QFont::Bold);
    }
    mergeFormatOnWordOrSelection(fmt);
    fontChanged(fmt.font());
}


void MainWindow::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

// set up format menu
void MainWindow::setupFormatMenu()
{


    formatMenu = new QMenu(tr("&Format"), menuBar);

    // color
    colorAct = new QAction(tr("C&olor"), this);
    formatMenu->addAction(colorAct);

    // font style
    fontStyleMenu = new QMenu(tr("Font Style"), formatMenu);

    // font
    boldAct = new QAction(QIcon(tr(":images/textbold.png")), tr("&Bold"), this);
    boldAct->setShortcut(Qt::CTRL + Qt::Key_B);
    boldAct->setCheckable(true);
    fontStyleMenu->addAction(boldAct);

    // italic
    italicAct = new QAction(QIcon(tr(":images/textitalic.png")), tr("&Italic"),
                            this);
    italicAct->setShortcut(Qt::CTRL + Qt::Key_I);
    italicAct->setCheckable(true);
    fontStyleMenu->addAction(italicAct);

    // underline
    underlineAct = new QAction(QIcon(tr(":images/textunder.png")),
                               tr("&Underline"), this);
    underlineAct->setShortcut(Qt::CTRL + Qt::Key_U);
    underlineAct->setCheckable(true);
    fontStyleMenu->addAction(underlineAct);

    formatMenu->addMenu(fontStyleMenu);

    // font
    fontAct = new QAction(QIcon(tr(":images/textfont.png")), tr("Font"), this);
    formatMenu->addAction(fontAct);

    menuBar->addMenu(formatMenu);
    setupFormatActions();
}

// set up format menu actions
void MainWindow::setupFormatActions()
{
    connect(colorAct, SIGNAL(triggered()), SLOT(textColor()));
    connect(boldAct, SIGNAL(triggered()), SLOT(textStyle()));
    connect(italicAct, SIGNAL(triggered()), SLOT(textStyle()));
    connect(underlineAct, SIGNAL(triggered()), SLOT(textStyle()));
    connect(comboFont, SIGNAL(activated(QString)),
            SLOT(textFontFamily(QString)));
    connect(comboStyle,SIGNAL(activated(int)),SLOT(textStyle(int)));
    connect(comboStyle, SIGNAL(activated(QString)),
            SLOT(updateTextStyleActs(QString)));
    connect(comboSize, SIGNAL(activated(QString)), SLOT(textSize(QString)));
    connect(fontAct, SIGNAL(triggered()), SLOT(textFont()));


}


void MainWindow::nextWindow()
{
    tabWidget->setCurrentIndex(tabWidget->currentIndex() + 1);
}


void MainWindow::previousWindow()
{
    tabWidget->setCurrentIndex(tabWidget->currentIndex() - 1);
}


void MainWindow::currentAllWindow()
{
    int i;
    QAction *a;
    int index = tabWidget->currentIndex();

    //delete previous Action
    foreach(a, openedFilesGrp->actions())
    {
        openedFilesGrp->removeAction(a);
        currentAllMenu->removeAction(a);
    }

    for (i = 0; i < tabWidget->count(); i++)
    {
        a = new QAction(tabWidget->tabText(i), this);
        a->setCheckable(true);
        if (i == index)
            a->setChecked(true);
        openedFilesGrp->addAction(a);
        currentAllMenu->addAction(a);
    }
    connect(openedFilesGrp, SIGNAL(triggered(QAction*)), this, SLOT(setCurrentWindow(QAction*)));
}


void MainWindow::setCurrentWindow(QAction *a)
{
    int index = openedFilesGrp->actions().indexOf(a);
    tabWidget->setCurrentIndex(index);
}


void MainWindow::openRecentFile()
{
    openFile((static_cast<QAction*>(sender()))->data().toString());
}


void MainWindow::fillRecentFileActs()
{
    for (int i = 0; i < config->maxRecentFiles; i++)
    {
        QAction *act = recentlyFilesMenu->addAction("", this,
                                                    SLOT(openRecentFile()));
        act->setVisible(false);
        recentFileActs << act;
    }
}

void MainWindow::updateRecentFiles()
{
    int i = 0;
    QStringListIterator it(config->recentFiles);
    while (it.hasNext())
    {
        QString fileName = it.next();
        if (QFileInfo(fileName).exists())
        {
            recentFileActs[i]->setText(
                        tr("&%1: %2--%3").arg(i + 1).arg(QFileInfo(fileName).fileName()).arg(
                            fileName));
            recentFileActs[i]->setData(fileName);
            recentFileActs[i]->setVisible(true);
            i++;
        }
        else
        {
            config->recentFiles.removeAll(fileName);
        }
    }
}

// set up window menu
void MainWindow::setupWindowMenu()
{
    windowMenu = new QMenu(tr("&Window"), menuBar);

    // next window
    nextAct = new QAction(QIcon(tr(":images/next.png")), tr("Next Window"), this);
    nextAct->setShortcut(Qt::CTRL + Qt::Key_Right);
    windowMenu->addAction(nextAct);
    topToolBar->addAction(nextAct);

    // previous window
    previousAct = new QAction(QIcon(tr(":images/previous.png")),
                              tr("Previous Window"), this);
    previousAct->setShortcut(Qt::CTRL + Qt::Key_Left);
    windowMenu->addAction(previousAct);
    topToolBar->addAction(previousAct);

    //recent file
    recentlyFilesMenu = new QMenu(tr("Recently Files"), windowMenu);
    fillRecentFileActs();
    windowMenu->addMenu(recentlyFilesMenu);

    // current all windows
    currentAllMenu = new QMenu(tr("Current Windows"), windowMenu);
    windowMenu->addMenu(currentAllMenu);
    openedFilesGrp = new QActionGroup(this);

    topToolBar->addSeparator();
    menuBar->addMenu(windowMenu);
    setupWindowActions();
}

// set up window menu actions
void MainWindow::setupWindowActions()
{
    connect(nextAct, SIGNAL(triggered()), SLOT(nextWindow()));
    connect(previousAct, SIGNAL(triggered()), SLOT(previousWindow()));
    connect(currentAllMenu, SIGNAL(aboutToShow()), SLOT(currentAllWindow()));
    connect(recentlyFilesMenu, SIGNAL(aboutToShow()), SLOT(updateRecentFiles()));
}

// debugger
void MainWindow::debugMenu()
{
    helpMenu = new QMenu(tr("&Debugger"), menuBar);

    //start debug
    actionStart = new QAction(QIcon(tr(":1/debugger.png")), tr("start debugging"), this);
    helpMenu->addAction(actionStart);
    topToolBar->addAction(actionStart);

    menuBar->addMenu(helpMenu);
    setupDebugActions();
}


void MainWindow::setupDebugActions() {
    connect(actionStart, SIGNAL(triggered()), this, SLOT(debug()));

}


// search
void MainWindow::search()
{
    if (searchDialog->isVisible())
        searchDialog->activateWindow();
    else
        searchDialog->show();
    connect(searchDialog, SIGNAL(search(QString, bool, bool, bool)),EDITOR,SLOT(search(QString, bool, bool, bool)));
    connect(searchDialog, SIGNAL(replace(QString, QString, bool, bool, bool)), EDITOR,SLOT(replace(QString, QString, bool, bool, bool)));
    connect(searchDialog, SIGNAL(replaceAll(QString, QString, bool, bool)),EDITOR,SLOT(replaceAll(QString, QString, bool, bool)));
}


MainWindow::~MainWindow()
{
    delete tabWidget;
    delete searchDialog;
    delete gotoLineDialog;
    delete perferenceDialog;

    delete comboStyle;
    delete comboFont;
    delete comboSize;

    delete openAct;
    delete newAct;
    delete saveAct;
    delete saveAsAct;
    delete saveAllAct;

    delete closeAct;
    delete closeAllAct;
    delete exitAct;

    delete copyAct;
    delete cutAct;
    delete pasteAct;
    delete undoAct;
    delete redoAct;
    delete selectAllAct;
    delete upperCaseAct;
    delete lowerCaseAct;
    delete findAct;
    delete gotoLineAct;
    delete preferenceAct;

    delete boldAct;
    delete italicAct;
    delete underlineAct;
    delete fontAct;
    delete colorAct;
    delete nextAct;
    delete previousAct;
    delete actionRun;
    delete actionStart;
    delete actionSingle_step;

    delete fileMenu;
    delete editMenu;
    delete fontStyleMenu;
    delete formatMenu;
    delete recentlyFilesMenu;
    delete currentAllMenu;
    delete windowMenu;
    delete helpMenu;
    delete menuBar;
    delete topToolBar;
    delete bottomToolBar;

    delete openedFilesGrp;
}
