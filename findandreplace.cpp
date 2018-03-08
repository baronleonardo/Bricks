#include "findandreplace.h"
#include <QMessageBox>

FindAndReplace::FindAndReplace(QPlainTextEdit* parent) : QWidget(parent) {
    this->editor = parent;
}

void FindAndReplace::find(QString exp) {
    int index;

    if(editor->document()->isEmpty())
        return;

    if( this->findExp_cursor.isNull() )
        this->findExp_cursor = editor->textCursor();

    // search
    index = editor->toPlainText().indexOf(exp, this->findExp_cursor.position());

    //if found match
    if(index != -1) {
        foundSearchMatch = true;
        canContinueSearchingFromTop = true;

        // mark the matched exp
        this->findExp_cursor.setPosition(index);
        this->findExp_cursor.setPosition( this->findExp_cursor.position() + exp.length(),
                                  QTextCursor::KeepAnchor );
        // select it
        editor->setTextCursor(this->findExp_cursor);
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
            QMessageBox::warning(editor, "Warning", "No match found");
            foundSearchMatch = false;
        }
    }
}

void FindAndReplace::findall(QString exp) {
    if(editor->document()->isEmpty())
        return;

    int index;
    QTextCursor cursor;
    QTextCharFormat findExp_format;
    findExp_format.setBackground(findExp_highlightColor);

    // move it the beginning of the document
    cursor = editor->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);

    // search
    index = editor->toPlainText().indexOf(exp, cursor.position());

    if(index == -1)
        QMessageBox::warning(editor, "Warning", "No match found");

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
            index = editor->toPlainText().indexOf(exp, cursor.position());
        }
    }
}

void FindAndReplace::replace(QString exp, QString replacement) {
    // replacement is done by finding and highlighting `exp` first
    // then replace it
    if(this->findExp_cursor.hasSelection())
        editor->insertPlainText(replacement);

    editor->find(exp);
}

void FindAndReplace::replaceall(QString exp, QString replacement) {
    // mark all change in replacement as once block to be undo at one time
    QTextCursor cursor = editor->textCursor();
    cursor.beginEditBlock();

    do {
        editor->find(exp);
        if(this->findExp_cursor.hasSelection())
            editor->insertPlainText(replacement);
    } while(this->canContinueSearchingFromTop == true);

    cursor.endEditBlock();
}

void FindAndReplace::clearSearchHighlight() {
    QTextCharFormat format;

    foreach (FindAndReplace::SearchResultProperties searchFoundProperties, searchResultList) {
        format.setBackground(searchFoundProperties.bg_color);
        searchFoundProperties.cursor.mergeCharFormat(format);
    }

    searchResultList.clear();
}


FindAndReplace::~FindAndReplace() {

}
