#include "notepad.h"
#include <QEvent>
#include<QResizeEvent>
#include<QPaintEvent>
#include<QKeyEvent>
#include<QMouseEvent>
#include<QContextMenuEvent>
#include <QFont>
#include <QMenu>
#include <QToolTip>
#include<QAction>
#include <QPainter>

NotePad::NotePad(Config *config, QWidget *parent) :
    QPlainTextEdit(parent), config(config)
{
    reconfig();

    extraArea = new QWidget(this);
    extraArea->installEventFilter(this);
    extraArea->setCursor(Qt::PointingHandCursor);

    QPalette extraAreaPalette(palette());
    extraAreaPalette.setColor(QPalette::Background, QColor(30, 60, 90));

    extraArea->setPalette(extraAreaPalette);
    extraArea->setAutoFillBackground(true);

    setLineWrapMode(QPlainTextEdit::NoWrap);
    setCursorWidth(2);
    blockCountChanged(0);
    setMouseTracking(true);

    QPalette plt = palette();
    plt.setColor(QPalette::Highlight, Qt::yellow);
    plt.setColor(QPalette::HighlightedText, Qt::blue);
    setPalette(plt);

    connect(this, SIGNAL(cursorPositionChanged()), SLOT(ensureCursorVisible()));
    connect(this, SIGNAL(blockCountChanged(int)), SLOT(blockCountChanged(int)));
    connect(document(), SIGNAL(contentsChange(int, int, int)), SLOT(contentsChange(int, int, int)));
    connect(config, SIGNAL(reread(int)), SLOT(reconfig(int)));
    connect(this, SIGNAL(updateRequest(QRect, int)), extraArea, SLOT(update()));
}

void NotePad::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    lineNumWidth = FONTWIDTH * lineNumDigits;
    int width = lineNumWidth + foldBoxWidth + 2 * foldBoxIndent;
    setViewportMargins(width, 0, 0, 0);
    extraArea->setGeometry(contentsRect().x(), contentsRect().y(), width,
                           contentsRect().height());
}

void NotePad::paintEvent(QPaintEvent *event)
{
    QPlainTextEdit::paintEvent(event);
    QPainter painter(viewport());
    painter.setPen(Qt::darkGray);
    QTextBlock block = firstVisibleBlock();
    QRectF rect;
    do
    {
        if (!block.isVisible())
            continue;
        rect = blockBoundingGeometry(block).translated(contentOffset());
        QTextLine line = block.layout()->lineAt(0);
        if (config->whitespaces)
        {
            QString txt = block.text();
            for (int i = 0; i < txt.length(); i++)
            {
                QPoint point(rect.x() + line.cursorToX(i),
                             rect.y() + line.ascent());
                if (txt[i] == ' ')
                    painter.drawText(point, QChar(0x00b7));
                else if (txt[i] == '\t')
                    painter.drawText(point, QChar(0x21b9));
            }
        }

        int state = block.userState();

        if (!(state & Error) && state & Folded)
        {
            QRect collapseRect(
                        rect.x() + line.rect().x() + line.naturalTextWidth()
                        + FONTWIDTH * 2, rect.y() + 2, FONTWIDTH * 6,
                        line.height() - 4);

            painter.drawText(collapseRect, Qt::AlignCenter,
                             state & Comment ? "...;" : "...)");
            painter.drawRoundedRect(collapseRect, 4, 6);
        }
    } while ((block = block.next()).isValid() && rect.y() < viewport()->height());
}

void NotePad::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return && event->modifiers() == Qt::SHIFT)
        return;

    if (event->key() == Qt::Key_Backspace && config->backUnindent)
    {
        QTextCursor cursor = textCursor();

        if (!cursor.hasSelection() && !cursor.atBlockStart()
                && cursor.block().text().left(cursor.positionInBlock()).trimmed().isEmpty())
        {
            cursor.movePosition(QTextCursor::PreviousCharacter,
                                QTextCursor::KeepAnchor,
                                qMin(config->indentSize, cursor.positionInBlock()));
            cursor.removeSelectedText();
            return;
        }
    }

    if (event->key() == Qt::Key_Tab && config->tabIndents)
    {
        QTextCursor cursor = textCursor();

        if (cursor.hasSelection())
        {

            QTextBlock block = document()->findBlock(cursor.selectionStart());

            int end = document()->findBlock(cursor.selectionEnd()).blockNumber();

            if (end - block.blockNumber())
            {
                cursor.beginEditBlock();
                do
                {
                    cursor.setPosition(block.position(), QTextCursor::MoveAnchor);
                    cursor.insertText(QString().fill(' ', config->indentSize));
                } while ((block = block.next()).isValid()
                         && block.blockNumber() <= end);
                cursor.endEditBlock();
                return;
            }

        }
        else if (textCursor().block().text().left(textCursor().positionInBlock()).trimmed().isEmpty())
        {
            textCursor().insertText(QString().fill(' ', config->indentSize));
            return;
        }
    }

    if (event->key() == Qt::Key_Tab && config->spaceTabs)
    {
        textCursor().insertText(QString().fill(' ', config->tabSize));
        return;
    }

    QPlainTextEdit::keyPressEvent(event);

    if (event->key() == Qt::Key_Return && config->autoIndent)
    {
        int state = textCursor().block().userState();

        if (!(state & Error) && (state & Nested))
        {
            QString txt = textCursor().block().previous().text();
            int i = 0;
            while (txt[i].isSpace())
                ++i;
            int previousBlockState = textCursor().block().previous().userState();
            if (!(previousBlockState & Error) && previousBlockState & Begin)
                i += config->indentSize;
            textCursor().insertText(QString().fill(' ', i));
        }
    }
}

void NotePad::mouseMoveEvent(QMouseEvent *event)
{
    QTextBlock block = findBlockByY(event->pos().y());

    QRect collapseRect;

    if (block.isValid())
    {
        QRectF rect = blockBoundingGeometry(block).translated(contentOffset());
        QTextLine line = block.layout()->lineAt(0);

        collapseRect = QRect(
                    rect.x() + line.rect().x() + line.naturalTextWidth()
                    + FONTWIDTH * 2, rect.y() + 2, FONTWIDTH * 6,
                    line.height() - 4);
    }

    int state = block.userState();

    if (!(state & Error) && state & Folded && collapseRect.contains(event->pos()))
    {
        pointedBlock = block;

        viewport()->setCursor(Qt::PointingHandCursor);

        QString str;

        while ((block = block.next()).isValid() && !block.isVisible())
        {

            if (str.count() > 1)
                str += "\n";

            if (block.blockNumber() - pointedBlock.blockNumber() > 50)
            {
                str += "..."; // "\n...";
                break;
            }

            str += block.text();
        }

        QToolTip::showText(event->globalPos(), str, this);
    }
    else
    {
        pointedBlock = QTextBlock();
        viewport()->setCursor(Qt::IBeamCursor);
    }

    QPlainTextEdit::mouseMoveEvent(event);
}

void NotePad::mousePressEvent(QMouseEvent *event)
{
    if (pointedBlock.isValid())
    {
        pointedBlock = QTextBlock();
        viewport()->setCursor(Qt::IBeamCursor);

        FULLRESIZE;
    }
    else
    {
        QPlainTextEdit::mousePressEvent(event);
    }
}

void NotePad::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    QAction *act = menu.addAction(tr("&Undo"), this, SLOT(undo())); //, QKeySequence::Undo);
    act->setEnabled(document()->isUndoAvailable());
    act = menu.addAction(tr("&Redo"), this, SLOT(redo()));  //, QKeySequence::Redo);
    act->setEnabled(document()->isRedoAvailable());
    menu.addSeparator();
    act = menu.addAction(tr("Cu&t"), this, SLOT(cut()));     //, QKeySequence::Cut);
    act->setEnabled(textCursor().hasSelection());
    act = menu.addAction(tr("&Copy"), this, SLOT(copy()));  //, QKeySequence::Copy);
    act->setEnabled(textCursor().hasSelection());
    act = menu.addAction(tr("&Paste"), this, SLOT(paste())); //, QKeySequence::Paste);
    act->setEnabled(canPaste());
    act = menu.addAction(tr("Delete"), this, SLOT(deleteSelected())); //, QKeySequence::Delete);
    act->setEnabled(textCursor().hasSelection());
    act = menu.addAction(tr("Select All"), this, SLOT(selectAll())); //, QKeySequence::SelectAll);
    act->setEnabled(!document()->isEmpty());
    menu.addSeparator();
    act = menu.addAction(tr("UpperCase"), this, SLOT(toUpperCase()));
    act->setEnabled(textCursor().hasSelection());
    act = menu.addAction(tr("LowerCase"), this, SLOT(toLowerCase()));
    act->setEnabled(textCursor().hasSelection());

    menu.exec(event->globalPos());
}

bool NotePad::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::Paint:
        extraAreaPaintEvent();
        return true;
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonDblClick:
        extraAreaMouseEvent(static_cast<QMouseEvent *>(event));
        return true;
    default:
        return QPlainTextEdit::eventFilter(obj, event);
    }
}

void NotePad::extraAreaPaintEvent()
{
    QTextBlock block = firstVisibleBlock();

    QPainter painter(extraArea);
    QFont font = painter.font();

    int y = 0;
    int cx = lineNumWidth + foldBoxIndent + foldBoxWidth / 2;

    painter.setPen(Qt::gray);

    do
    {
        if (!block.isVisible())
            continue;

        QRectF rect = blockBoundingGeometry(block);

        y = rect.translated(contentOffset()).y();

        if (block == textCursor().block())
        {
            painter.setPen(Qt::yellow);
            font.setBold(true);
        }
        else
        {
            font.setBold(false);
        }

        painter.setFont(font);
        if (config->showLineNumber)
        {
            painter.drawText(0, y, lineNumWidth, fontMetrics().height(),
                             Qt::AlignRight, QString::number(block.blockNumber() + 1));

            painter.setPen(Qt::gray);
        }

        int state = block.userState();

        int cy = y + fontMetrics().height() / 2;

        if (!(state & Error)
                && !(state & Begin && state & End && !(state & Nested)))
        {

            if (state & Begin)
            {

                if (state & Comment)
                    painter.drawEllipse(FOLDBOXRECT(cy));
                else
                    painter.drawRoundedRect(FOLDBOXRECT(cy), 7, 3);

                if (!(!(state & Nested) && state & Folded))
                    painter.drawLine(cx, cy + foldBoxWidth / 2, cx,
                                     y + rect.height());

                if (state & Nested)
                    painter.drawLine(cx, y, cx, cy - foldBoxWidth / 2);

                painter.drawLine(cx - foldBoxLength, cy, cx + foldBoxLength, cy);

                if (state & Folded)
                    painter.drawLine(cx, cy - foldBoxLength, cx,
                                     cy + foldBoxLength);
            }
            else if (state & End)
            {
                painter.drawLine(cx, cy, cx + foldBoxLength, cy);

                if (state & Nested)
                    painter.drawLine(cx, y, cx, y + rect.height());
                else
                    painter.drawLine(cx, y, cx, cy);
            }
            else if (state & Nested)
            {
                painter.drawLine(cx, y, cx, y + rect.height());
            }
        }

        if (state != Empty && state & Rehighligh)
        {
            block.setUserState(state & ~Rehighligh);
        }

    } while ((block = block.next()).isValid() && y < viewport()->height());
}

void NotePad::extraAreaMouseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QTextBlock block = findBlockByY(event->pos().y());

        if (FOLDBOXRECT(blockBoundingGeometry(block).translated(contentOffset()).y() +
                        fontMetrics().height() / 2).contains(event->pos()))
        {
            ensureCursorVisible();
            FULLRESIZE;
        }
    }
}

// set block state
int NotePad::setBlockState(QTextBlock &block)
{
    int previousBlockState = block.previous().userState();

    if (previousBlockState & Error)
        previousBlockState = 0;

    bool inString = previousBlockState & String;

    int previousBraceDepth = previousBlockState >> StateShift;

    int braceDepth = previousBraceDepth;

    QString txt = block.text();

    int state = 0;
    int i = 0;

    while (txt[i].isSpace())
        ++i;

    if (txt[i] == ';')
    {

        if (!previousBraceDepth || previousBlockState & Comment)
        {
            state |= Comment;
            if (previousBlockState & Comment)
            {

                if (braceDepth)
                {
                    braceDepth--;
                }
                else
                {
                    previousBraceDepth = 1;

                    previousBlockState = (previousBlockState & End ? Nested : Begin)
                            | (previousBlockState & Folded)
                            | (previousBlockState & Rehighligh);

                    previousBlockState |= (previousBraceDepth << StateShift)
                            | Comment;

                    block.previous().setUserState(previousBlockState);
                }

                state |= End;
            }
        }

    }
    else
    {
        if (previousBlockState & Comment && previousBlockState & (Nested | Begin))
        {
            previousBraceDepth = 0;
            braceDepth = 0;

            previousBlockState = previousBlockState & Nested ? End : 0;
            previousBlockState |= (previousBraceDepth << StateShift) | Comment;

            block.previous().setUserState(previousBlockState);
        }

        if (txt[i].isNull() && !previousBraceDepth)
        {
            state = Empty;
        }
        else if (txt[i] != '(' && !previousBraceDepth)
        {
            braceDepth = i;
            state |= Error;
        }
        else
        {
            while (i < txt.length())
            {

                if (txt[i] == '"' && !(i && txt[i - 1] == '\\'))
                {
                    inString = !inString;
                }
                else if (!inString)
                {

                    if (txt[i] == ';')
                        break;

                    if (txt[i] == '(')
                        braceDepth++;
                    else if (txt[i] == ')')
                        braceDepth--;

                    if (braceDepth == -1)
                    {
                        braceDepth = i;
                        state |= Error;
                        break;
                    }
                }

                i++;
            }
        }
    }

    if (!(state & (Error | Comment)))
    {

        if (inString)
            state |= String;

        if (braceDepth > previousBraceDepth || !previousBraceDepth)
            state |= Begin;

        if (braceDepth < previousBraceDepth || !braceDepth)
            state |= End;

        if (previousBraceDepth && braceDepth)
            state |= Nested;
    }

    state |= (braceDepth << StateShift);

    int previousState = block.userState();

    if (previousState != Empty)
        state |= (previousState & Folded) | (previousState & Rehighligh);

    block.setUserState(state);

    return state;
}


void NotePad::contentsChange(int pos, int, int)
{
    QTextBlock block = textCursor().block();

    if (block.userState() == Empty)
        block.setUserState(block.previous().userState() | Rehighligh);

    block = document()->findBlock(pos);

    int startBlockNum = block.blockNumber();

    while (block.isValid())
    {
        int previousState = block.userState();
        int state = setBlockState(block);

        if (!(previousState & Error) && !(state & Error) && previousState & Comment
                && state & Comment)
        {

            QTextBlock next = block.next();

            int nextBlockState = next.userState();
            setBlockState(next);
            next.setUserState(nextBlockState);
            state = block.userState();
        }

        if (block.blockNumber() > startBlockNum && state & Begin
                && !(state & Nested))
        {
            break;
        }

        block.setUserState(state | Rehighligh);

        block = block.next();
    }
}


QTextBlock NotePad::findBlockByY(int y)
{
    QTextBlock block = firstVisibleBlock();

    do
    {
        if (blockBoundingGeometry(block).contains(0, y))
            return block;

    } while ((block = block.next()).isValid());

    return QTextBlock();
}


void NotePad::ensureCursorVisible()
{
    QTextCursor cursor = textCursor();

    QTextBlock block = document()->findBlock(cursor.position());

    if (!block.isVisible())
    {

        while (!(block = block.previous()).isVisible())
            ;

        cursor.setPosition(block.position(), QTextCursor::MoveAnchor);
        setTextCursor(cursor);
    }
}


void NotePad::reconfig(int receiver)
{
    if (!(receiver & Config::Editor))
        return;

    QFont font(config->fontFamily, config->fontSize);
    font.setFixedPitch(true);

    foldBoxWidth = config->fontSize;
    foldBoxIndent = foldBoxWidth / 2;
    foldBoxLength = foldBoxWidth / 6;

    setFont(font);
    QToolTip::setFont(font);

    setTabStopWidth(FONTWIDTH * config->tabSize);

    if (!(receiver & Config::Init))
    {

        QTextBlock block = document()->firstBlock();

        do
        {
            block.setUserState(block.userState() | Rehighligh);
        } while ((block = block.next()).isValid());

        FULLRESIZE;
    }
}


int NotePad::search(QString str, bool backward, bool matchCase, bool regExp)
{
    QTextDocument::FindFlags options;

    if (backward)
        options = QTextDocument::FindBackward;

    if (matchCase)
        options |= QTextDocument::FindCaseSensitively;

    QTextCursor cursor = textCursor();

    cursor =
            regExp ?
                document()->find(QRegExp(str), cursor, options) :
                document()->find(str, cursor, options);

    if (cursor.isNull())
        return false;

    setTextCursor(cursor);
    return true;
}


void NotePad::replace(QString str1, QString str2, bool backward, bool matchCase,
                      bool regExp)
{
    QTextCursor cursor = textCursor();

    if (!cursor.hasSelection())
    {
        search(str1, backward, matchCase, regExp);
    }
    else
    {
        int pos = textCursor().position() - textCursor().selectedText().length();

        cursor.beginEditBlock();
        cursor.insertText(str2);
        cursor.endEditBlock();

        if (backward)
        {
            cursor.setPosition(pos, QTextCursor::MoveAnchor);
            setTextCursor(cursor);
        }
    }
}


void NotePad::replaceAll(QString str1, QString str2, bool matchCase,
                         bool regExp)
{
    QTextCursor cursor = textCursor();

    cursor.setPosition(0, QTextCursor::MoveAnchor);
    setTextCursor(cursor);

    while (search(str1, false, matchCase, regExp))
        replace(str1, str2, false, matchCase, regExp);
}


void NotePad::gotoLine(int lineNumber)
{
    QTextCursor cursor = textCursor();
    QTextBlock block = document()->findBlockByLineNumber(lineNumber - 1);
    cursor.setPosition(block.position());
    setTextCursor(cursor);
}

NotePad::~NotePad()
{
    delete extraArea;
}
