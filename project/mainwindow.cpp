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

MainWindow::MainWindow(Config *config, QWidget *parent) :
    QMainWindow(parent), config(config)
{
    init();
    setupFileMenu();
    setupEditMenu();
    setupFormatMenu();
    setupWindowMenu();
    setupHelpMenu();
    currentChanged(-1);
    currentChanged(0);
    setupEditActions();
    setCentralWidget(tabWidget);
    restoreGeometry(config->mainWindowsGeometry);
    restoreState(config->mainWindowState);
}

//初始化
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

//关闭事件
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

//文档发生改变
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

//文档选中状态发生改变
void MainWindow::selectionChanged()
{
    upperCaseAct->setEnabled(EDITOR->textCursor().hasSelection());
    lowerCaseAct->setEnabled(EDITOR->textCursor().hasSelection());
    QTextCharFormat fmt=EDITOR->currentCharFormat();
    currentCharFormatChanged(fmt);
}

//文档段落发生改变
void MainWindow::blockCountChanged(int blockCount)
{
    gotoLineDialog->setMaxLineNumber(blockCount);
}

//Tab发生改变时触发的槽
void MainWindow::currentChanged(int index)
{
    if (index == -1)
    {
        newNumber = 0;
        if (config->showReadme)
            showReadme();
        else
            newFile();
        updateTextStyleActs(config->fontStyle);
        return;
    }
    updateActions();
    setWindowIcon(QIcon(tr(":images/notepad.png")));
    setWindowTitle(tr("NotePad(%1)").arg(openedFiles.at(index)));
}

//创建新的Tab（用于打开文件）
void MainWindow::newTab(const QString& fileName, QFile& file)
{
    openedFiles << fileName;
    NotePad *notePad = new NotePad(config);
    tabWidget->addTab(notePad, QFileInfo(fileName).fileName());
    QByteArray data = file.readAll();
    notePad->setPlainText(QString::fromLocal8Bit(data));
    tabWidget->setCurrentWidget(notePad);
}

//显示readme文件
void MainWindow::showReadme()
{
    QString readmeFile = QApplication::applicationDirPath();
    readmeFile += readmeFile.endsWith('/') ? "readme.txt" : "/readme.txt";
    if (!QFile::exists(readmeFile))
    {
        QFile file(readmeFile);
        file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
        QTextStream stream(&file);
        stream
                << "/*===========================================================================\n"
                << "NotePad, editor for Common Use.\n"
                << "Copyright (C) 2012 Zouxingyu\n" << "\n"
                << "This program is free software: you can redistribute it and/or modify\n"
                << "it under the terms of the GNU General Public License as published by\n"
                << "the Free Software Foundation, either version 3 of the License, or\n"
                << "(at your option) any later version.\n" << "\n"
                << "This program is distributed in the hope that it will be useful,\n"
                << "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
                << "GNU General Public License for more details.\n" << "\n"
                << "You should have received a copy of the GNU General Public License\n"
                << "along with this program.  If not, see <http://www.gnu.org/licenses/>\n"
                << "===========================================================================*/";
        file.close();
    }
    QFile rfile(readmeFile);
    rfile.open(QFile::ReadOnly);
    newTab(readmeFile, rfile);
}

//判断指定文件是否需要保存
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

//更新config中最近打开的文件列表
void MainWindow::updateRecentFilesList()
{
    int index = tabWidget->currentIndex();
    QString fileName = openedFiles.at(index);
    config->recentFiles.removeAll(fileName);
    config->recentFiles.prepend(fileName);
    if (config->recentFiles.size() > config->maxRecentFiles)
        config->recentFiles.removeLast();
}

//更新action的状态(子函数)
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

//更新各Action的状态
void MainWindow::updateActions()
{
    connect(EDITOR, SIGNAL(modificationChanged(bool)), SLOT(modificationChanged(bool)), Qt::UniqueConnection);
    connect(EDITOR,SIGNAL(selectionChanged()),SLOT(selectionChanged()));
    connect(EDITOR,SIGNAL(blockCountChanged(int)),SLOT(blockCountChanged(int)));
    refreshActions();
    updateRecentFilesList();
}

//关闭已经重复打开的文件
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

//设置文本格式（包括字体、颜色、风格）
void MainWindow::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = EDITOR->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    EDITOR->mergeCurrentCharFormat(format);
}

//打开文件
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

//打开文件
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

//新建文件
void MainWindow::newFile()
{
    QString fileName = tr("New %1").arg(++newNumber);
    openedFiles << fileName;
    tabWidget->setCurrentIndex(
                tabWidget->addTab(new NotePad(config), fileName));
}

//文件另存为
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

//保存文件
bool MainWindow::fileSave(int index)
{
    NotePad *notePad = static_cast<NotePad*>(tabWidget->widget(index));
    QString fileName = openedFiles.at(index);
    if (!fileName.contains("/") && !fileName.contains("\\"))
        return fileSaveAs(index);
    QTextDocumentWriter writer(fileName);
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

//文件另存为（保存当前文件）
bool MainWindow::fileSaveAs()
{
    return fileSaveAs(tabWidget->currentIndex());
}

//保存文件（保存当前文件）
bool MainWindow::fileSave()
{
    return fileSave(tabWidget->currentIndex());
}

//保存所有文件
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

//打印文件
void MainWindow::filePrint()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (EDITOR->textCursor().hasSelection())
        dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    dlg->setWindowTitle(tr("Print Document"));
    if (dlg->exec() == QDialog::Accepted)
    {
        EDITOR->print(&printer);
    }
    delete dlg;
#endif
}

//打印预览
void MainWindow::filePrintPreview()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreview(QPrinter*)));
    preview.exec();
#endif
}

//输出成PDF
void MainWindow::filePrintPdf()
{
#ifndef QT_NO_PRINTER
    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF",
                                                    QString(), "*.pdf");
    if (!fileName.isEmpty())
    {
        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".pdf");
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        EDITOR->document()->print(&printer);
    }
#endif
}

//打印预览子函数
void MainWindow::printPreview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    EDITOR->print(printer);
#endif
}

//关闭文件（指定文件）
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

//关闭文件（当前文件）
void MainWindow::fileClose()
{
    fileClose(tabWidget->currentIndex());
}

//关闭所有文件
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

//文件菜单功能实现
void MainWindow::setupFileMenu()
{
    fileMenu = new QMenu(tr("&File"), menuBar);

    //打开文件
    openAct = new QAction(QIcon(tr(":images/fileopen.png")), tr("&Open"), this);
    openAct->setShortcut(QKeySequence::Open);
    fileMenu->addAction(openAct);
    topToolBar->addAction(openAct);

    //新建文件
    newAct = new QAction(QIcon(tr(":images/filenew.png")), tr("&New"), this);
    newAct->setShortcut(QKeySequence::New);
    fileMenu->addAction(newAct);
    topToolBar->addAction(newAct);

    fileMenu->addSeparator();

    //保存文件
    saveAct = new QAction(QIcon(tr(":images/filesave.png")), tr("&Save"), this);
    saveAct->setShortcut(QKeySequence::Save);
    fileMenu->addAction(saveAct);
    topToolBar->addAction(saveAct);

    //文件另存为
    saveAsAct = new QAction(QIcon(tr(":images/filesaveas.png")),
                            tr("Save &As..."), this);
    saveAsAct->setShortcut(QKeySequence::SaveAs);
    fileMenu->addAction(saveAsAct);
    topToolBar->addAction(saveAsAct);

    //保存所有
    saveAllAct = new QAction(QIcon(tr(":images/saveall.png")), tr("Save All"),
                             this);
    saveAllAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
    fileMenu->addAction(saveAllAct);
    topToolBar->addAction(saveAllAct);

    fileMenu->addSeparator();

    //打印
    printAct = new QAction(QIcon(tr(":images/fileprint.png")), tr("Print"),
                           this);
    printAct->setShortcut(QKeySequence::Print);
    fileMenu->addAction(printAct);
    topToolBar->addAction(printAct);

    //打印预览
    printPreviewAct = new QAction(QIcon(tr(":images/printpreview.png")),
                                  tr("Print Pre&view"), this);
    fileMenu->addAction(printPreviewAct);
    topToolBar->addAction(printPreviewAct);

    //导出成PDF
    exportPdfAct = new QAction(QIcon(tr(":images/exportpdf.png")),
                               tr("Export Pdf"), this);
    fileMenu->addAction(exportPdfAct);
    topToolBar->addAction(exportPdfAct);

    fileMenu->addSeparator();

    //关闭当前文件
    closeAct = new QAction(QIcon(tr(":images/fileclose.png")), tr("Close"),
                           this);
    closeAct->setShortcut(QKeySequence::Close);
    fileMenu->addAction(closeAct);
    topToolBar->addAction(closeAct);

    //关闭所有文件
    closeAllAct = new QAction(QIcon(tr(":images/closeall.png")),
                              tr("Close All"), this);
    closeAllAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_X);
    fileMenu->addAction(closeAllAct);
    topToolBar->addAction(closeAllAct);

    //退出
    exitAct = new QAction(QIcon(tr(":images/fileexit.png")), tr("Exit"), this);
    exitAct->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(exitAct);
    topToolBar->addAction(exitAct);

    topToolBar->addSeparator();

    menuBar->addMenu(fileMenu);
    setupFileActions();
}

//文件菜单Action设置
void MainWindow::setupFileActions()
{
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(fileSave()));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
    connect(saveAllAct, SIGNAL(triggered()), this, SLOT(fileSaveAll()));
#ifndef QT_NO_PRINTER
    connect(printAct, SIGNAL(triggered()), this, SLOT(filePrint()));
    connect(printPreviewAct, SIGNAL(triggered()), this,
            SLOT(filePrintPreview()));
    connect(exportPdfAct, SIGNAL(triggered()), this, SLOT(filePrintPdf()));
#endif
    connect(closeAct, SIGNAL(triggered()), this, SLOT(fileClose()));
    connect(closeAllAct, SIGNAL(triggered()), this, SLOT(fileCloseAll()));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
}

//转到行
void MainWindow::gotoLine()
{
    if (gotoLineDialog->isVisible())
        gotoLineDialog->activateWindow();
    else
        gotoLineDialog->show();
    connect(gotoLineDialog,SIGNAL(gotoLine(int)),EDITOR,SLOT(gotoLine(int)));
}

//编辑菜单功能实现
void MainWindow::setupEditMenu()
{
    editMenu = new QMenu(tr("&Edit"), menuBar);

    //复制
    copyAct = new QAction(QIcon(tr(":images/editcopy.png")), tr("&Copy"), this);
    copyAct->setShortcut(QKeySequence::Copy);
    editMenu->addAction(copyAct);
    topToolBar->addAction(copyAct);

    //剪切
    cutAct = new QAction(QIcon(tr(":images/editcut.png")), tr("&Cut"), this);
    cutAct->setShortcut(QKeySequence::Cut);
    editMenu->addAction(cutAct);
    topToolBar->addAction(cutAct);

    //粘贴
    pasteAct = new QAction(QIcon(tr(":images/editpaste.png")), tr("&Paste"), this);
    pasteAct->setShortcut(QKeySequence::Paste);
    editMenu->addAction(pasteAct);
    topToolBar->addAction(pasteAct);

    editMenu->addSeparator();

    //撤销
    undoAct = new QAction(QIcon(tr(":images/editundo.png")), tr("&Undo"), this);
    undoAct->setShortcut(QKeySequence::Undo);
    editMenu->addAction(undoAct);
    topToolBar->addAction(undoAct);

    //重做
    redoAct = new QAction(QIcon(tr(":images/editredo.png")), tr("&Redo"), this);
    redoAct->setShortcut(QKeySequence::Redo);
    editMenu->addAction(redoAct);
    topToolBar->addAction(redoAct);

    editMenu->addSeparator();

    //全选
    selectAllAct = new QAction(QIcon(tr(":images/editselectall.png")),
                               tr("&Select all"), this);
    selectAllAct->setShortcut(QKeySequence::SelectAll);
    editMenu->addAction(selectAllAct);
    topToolBar->addAction(selectAllAct);

    //转为大写
    upperCaseAct = new QAction(QIcon(tr(":images/uppercase.png")), tr("UpperCase"),
                               this);
    editMenu->addAction(upperCaseAct);
    topToolBar->addAction(upperCaseAct);

    //转为小写
    lowerCaseAct = new QAction(QIcon(tr(":images/lowercase.png")), tr("LowerCase"),
                               this);
    editMenu->addAction(lowerCaseAct);
    topToolBar->addAction(lowerCaseAct);

    editMenu->addSeparator();

    //转到行
    gotoLineAct = new QAction(tr("Goto Line..."), this);
    gotoLineAct->setShortcut(Qt::CTRL + Qt::Key_G);
    editMenu->addAction(gotoLineAct);

    //查找和替换
    findAct = new QAction(QIcon(tr(":images/editfind.png")), tr("&Find/Replace"),
                          this);
    findAct->setShortcut(QKeySequence::Find);
    editMenu->addAction(findAct);
    topToolBar->addAction(findAct);

    //首选项
    preferenceAct = new QAction(tr("Preference"), this);
    editMenu->addAction(preferenceAct);

    topToolBar->addSeparator();

    menuBar->addMenu(editMenu);
}

//编辑菜单Action设置
void MainWindow::setupEditActions()
{
    connect(copyAct, SIGNAL(triggered()), EDITOR,SLOT(copy()));
    connect(pasteAct,SIGNAL(triggered()),EDITOR,SLOT(paste()));
    connect(undoAct,SIGNAL(triggered()),EDITOR,SLOT(undo()));
    connect(redoAct,SIGNAL(triggered()),EDITOR,SLOT(redo()));
    connect(selectAllAct,SIGNAL(triggered()),EDITOR,SLOT(selectAll()));
    connect(upperCaseAct,SIGNAL(triggered()),EDITOR,SLOT(toUpperCase()));
    connect(lowerCaseAct,SIGNAL(triggered()),EDITOR,SLOT(toLowerCase()));
    connect(gotoLineAct,SIGNAL(triggered()),this,SLOT(gotoLine()));
    connect(findAct,SIGNAL(triggered()),this,SLOT(search()));
    connect(preferenceAct,SIGNAL(triggered()),this,SLOT(perference()));
}

//设置文本颜色
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

//设置文本字体（通过字体组合框）
void MainWindow::textFontFamily(const QString& font)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(font);
    currentCharFormatChanged(fmt);
    mergeFormatOnWordOrSelection(fmt);
}

//设置文本字体
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

//设置字体大小
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

//文本颜色发生改变
void MainWindow::colorChanged(const QColor &col)
{
    QPixmap pix(16, 16);
    pix.fill(col);
    colorAct->setIcon(pix);
}

//字体发生改变
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

//更新ComboStyle下拉列表框的值
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

//设置文本风格
void MainWindow::textStyle()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(boldAct->isChecked() ? QFont::Bold : QFont::Normal);
    fmt.setFontItalic(italicAct->isChecked());
    fmt.setFontUnderline(underlineAct->isChecked());
    mergeFormatOnWordOrSelection(fmt);
    fontChanged(fmt.font());
}

//设置字体风格Action是否被check
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

//设置文本风格
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

//文本格式发生改变
void MainWindow::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

//格式菜单功能实现
void MainWindow::setupFormatMenu()
{
    formatMenu = new QMenu(tr("&Format"), menuBar);

    //字体颜色
    colorAct = new QAction(tr("C&olor"), this);
    formatMenu->addAction(colorAct);

    //字体风格
    fontStyleMenu = new QMenu(tr("Font Style"), formatMenu);

    //粗体
    boldAct = new QAction(QIcon(tr(":images/textbold.png")), tr("&Bold"), this);
    boldAct->setShortcut(Qt::CTRL + Qt::Key_B);
    boldAct->setCheckable(true);
    fontStyleMenu->addAction(boldAct);

    //斜体
    italicAct = new QAction(QIcon(tr(":images/textitalic.png")), tr("&Italic"),
                            this);
    italicAct->setShortcut(Qt::CTRL + Qt::Key_I);
    italicAct->setCheckable(true);
    fontStyleMenu->addAction(italicAct);

    //下划线
    underlineAct = new QAction(QIcon(tr(":images/textunder.png")),
                               tr("&Underline"), this);
    underlineAct->setShortcut(Qt::CTRL + Qt::Key_U);
    underlineAct->setCheckable(true);
    fontStyleMenu->addAction(underlineAct);

    formatMenu->addMenu(fontStyleMenu);
    topToolBar->addActions(fontStyleMenu->actions());

    //字体
    fontAct = new QAction(QIcon(tr(":images/textfont.png")), tr("Font"), this);
    formatMenu->addAction(fontAct);
    topToolBar->addAction(fontAct);

    topToolBar->addSeparator();

    menuBar->addMenu(formatMenu);
    setupFormatActions();
}

//格式菜单Action设置
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

//下一个窗口
void MainWindow::nextWindow()
{
    tabWidget->setCurrentIndex(tabWidget->currentIndex() + 1);
}

//前一个窗口
void MainWindow::previousWindow()
{
    tabWidget->setCurrentIndex(tabWidget->currentIndex() - 1);
}

//当前所有窗口
void MainWindow::currentAllWindow()
{
    int i;
    QAction *a;
    int index = tabWidget->currentIndex();

    //删除原先的Action
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

//窗口菜单中的项被选中时触发的槽
void MainWindow::setCurrentWindow(QAction *a)
{
    int index = openedFilesGrp->actions().indexOf(a);
    tabWidget->setCurrentIndex(index);
}

//打开最近的文档
void MainWindow::openRecentFile()
{
    openFile((static_cast<QAction*>(sender()))->data().toString());
}

//填充recentFileActs
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

//更新最近打开的文件菜单
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

//窗口菜单功能实现
void MainWindow::setupWindowMenu()
{
    windowMenu = new QMenu(tr("&Window"), menuBar);

    //下一个窗口
    nextAct = new QAction(QIcon(tr(":images/next.png")), tr("Next Window"), this);
    nextAct->setShortcut(Qt::CTRL + Qt::Key_Right);
    windowMenu->addAction(nextAct);
    topToolBar->addAction(nextAct);

    //上一个窗口
    previousAct = new QAction(QIcon(tr(":images/previous.png")),
                              tr("Previous Window"), this);
    previousAct->setShortcut(Qt::CTRL + Qt::Key_Left);
    windowMenu->addAction(previousAct);
    topToolBar->addAction(previousAct);

    //最近关闭的文件
    recentlyFilesMenu = new QMenu(tr("Recently Files"), windowMenu);
    fillRecentFileActs();
    windowMenu->addMenu(recentlyFilesMenu);

    //当前所有窗口
    currentAllMenu = new QMenu(tr("Current Windows"), windowMenu);
    windowMenu->addMenu(currentAllMenu);
    openedFilesGrp = new QActionGroup(this);

    topToolBar->addSeparator();
    menuBar->addMenu(windowMenu);
    setupWindowActions();
}

//窗口菜单Action设置
void MainWindow::setupWindowActions()
{
    connect(nextAct, SIGNAL(triggered()), SLOT(nextWindow()));
    connect(previousAct, SIGNAL(triggered()), SLOT(previousWindow()));
    connect(currentAllMenu, SIGNAL(aboutToShow()), SLOT(currentAllWindow()));
    connect(recentlyFilesMenu, SIGNAL(aboutToShow()), SLOT(updateRecentFiles()));
}

//帮助菜单功能实现
void MainWindow::setupHelpMenu()
{
    helpMenu = new QMenu(tr("&Help"), menuBar);

    //关于本软件
    aboutAct = new QAction(QIcon(tr(":images/helpabout.png")), tr("About"), this);
    helpMenu->addAction(aboutAct);
    topToolBar->addAction(aboutAct);

    //关于Qt
    aboutQtAct = new QAction(QIcon(tr(":images/helpaboutqt.png")), tr("About Qt"),
                             this);
    helpMenu->addAction(aboutQtAct);
    topToolBar->addAction(aboutQtAct);

    menuBar->addMenu(helpMenu);
    setupHelpActions();
}

//帮助Action设置
void MainWindow::setupHelpActions()
{
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

//关于本软件
void MainWindow::about()
{
    QMessageBox::about(this, tr("About"), tr("This example demonstrates Qt's "
                                             "rich text editing facilities in action, providing an example "
                                             "document for you to experiment with."));
}

//查找
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

//首选项
void MainWindow::perference()
{
    if (perferenceDialog->isVisible())
        perferenceDialog->activateWindow();
    else
        perferenceDialog->show();
    perferenceDialog->exec();
    comboFont->setCurrentFont(QFont(config->fontFamily));
    comboStyle->setCurrentIndex(comboStyle->findText(config->fontStyle));
    comboSize->setCurrentIndex(comboSize->findText(tr("%1").arg(config->fontSize)));
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
    delete printAct;
    delete printPreviewAct;
    delete exportPdfAct;
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
    delete aboutAct;
    delete aboutQtAct;

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
