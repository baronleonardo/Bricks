#include "editor.h"
#include <QMessageBox>
#include <QTextCharFormat>

Editor::Editor(QWidget *parent) :
    QTextEdit(parent) {
}

bool Editor::write(QTextStream* file_stream) {
    if(!file_stream->device()->isReadable())
        return false;

    // TODO: need optimizations
    this->setPlainText(file_stream->readAll());

    return true;
}

void Editor::find() {
//    lineSel.format.setBackground(Qt::blue);
//    this->extraSelections()[0].format.setBackground(Qt::blue);
    this->findall("Editor::~Editor()");
//    clearSearchHighlight();
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

void Editor::clearSearchHighlight() {
    QTextCharFormat format;

    foreach (Editor::SearchResultProperties searchFoundProperties, searchResultList) {
        format.setBackground(searchFoundProperties.bg_color);
        searchFoundProperties.cursor.mergeCharFormat(format);
    }
}

Editor::~Editor() {
}
