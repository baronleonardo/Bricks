#include "editor.h"
#include <QMessageBox>
#include <QTextCharFormat>
#include <QTextDocumentFragment>

Editor::Editor(QWidget *parent) :
    QPlainTextEdit(parent) {
    this->setLineWrapMode(QPlainTextEdit::NoWrap);
}

bool Editor::write(QTextStream* file_stream) {
    if(!file_stream->device()->isReadable())
        return false;

    // TODO: need optimizations
    this->setPlainText(file_stream->readAll());

    return true;
}

void Editor::find(QString exp) {
    int index;

    if(this->document()->isEmpty())
        return;

    if( this->findExp_cursor.isNull() )
        this->findExp_cursor = this->textCursor();

    // search
    index = this->toPlainText().indexOf(exp, this->findExp_cursor.position());

    //if found match
    if(index != -1) {
        foundSearchMatch = true;
        canContinueSearchingFromTop = true;

        // mark the matched exp
        this->findExp_cursor.setPosition(index);
        this->findExp_cursor.setPosition( this->findExp_cursor.position() + exp.length(),
                                  QTextCursor::KeepAnchor );
        // select it
        this->setTextCursor(this->findExp_cursor);
    }

    else {
        // try search from the top of the document
        if(canContinueSearchingFromTop) {
            canContinueSearchingFromTop = false;
            this->findExp_cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
            find(exp);
        }

        // show warning msg if no match at all
        else if(!foundSearchMatch) {
            QMessageBox::warning(this, "Warning", "No match found");
            foundSearchMatch = false;
        }
    }
}

void Editor::findall(QString exp) {
    if(this->document()->isEmpty())
        return;

    int index;
    QTextCursor cursor;
    QTextCharFormat findExp_format;
    findExp_format.setBackground(findExp_highlightColor);

    // move it the beginning of the document
    cursor = this->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);

    // search
    index = this->toPlainText().indexOf(exp, cursor.position());

    if(index == -1)
        QMessageBox::warning(this, "Warning", "No match found");

    else {
        while (index != -1) {
            // mark the matched exp
            cursor.setPosition(index);
            cursor.setPosition( cursor.position() + exp.length(),
                                QTextCursor::KeepAnchor );

            // save current search result properties for clearing it
            // after finishing searching
            searchResultList.append( { this->findExp_cursor.charFormat().background(),
                                       cursor } );

            // highlight it
            cursor.mergeCharFormat(findExp_format);

            // update search index
            index = this->toPlainText().indexOf(exp, cursor.position());
        }
    }
}

void Editor::replace(QString exp, QString replacement) {
    // replacement is done by finding and highlighting `exp` first
    // then replace it
    if(this->findExp_cursor.hasSelection())
        this->insertPlainText(replacement);

    this->find(exp);
}

void Editor::replaceall(QString exp, QString replacement) {
    // mark all change in replacement as once block to be undo at one time
    QTextCursor cursor = this->textCursor();
    cursor.beginEditBlock();

    do {
        this->find(exp);
        if(this->findExp_cursor.hasSelection())
            this->insertPlainText(replacement);
    } while(this->canContinueSearchingFromTop == true);

    cursor.endEditBlock();
}

void Editor::clearSearchHighlight() {
    QTextCharFormat format;

    foreach (Editor::SearchResultProperties searchFoundProperties, searchResultList) {
        format.setBackground(searchFoundProperties.bg_color);
        searchFoundProperties.cursor.mergeCharFormat(format);
    }
}

Editor::~Editor() {
}
