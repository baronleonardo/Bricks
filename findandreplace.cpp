#include "findandreplace.h"
#include <QMessageBox>

FindAndReplace::FindAndReplace(QPlainTextEdit* parent) : QWidget(parent) {
    this->editor = parent;
}

void FindAndReplace::setRgExMode(bool enable) {
    if(enable) {
        findPtr = &FindAndReplace::_findRegEx;
        findAllPtr = &FindAndReplace::_findRegEx;
    }

    else {
        findPtr = &FindAndReplace::_find;
        findAllPtr = &FindAndReplace::_find;
    }
}

void FindAndReplace::setCaseSensitive(bool enable) {
    if(enable) {
        sensitivity = Qt::CaseSensitive;
        this->flags |= QTextDocument::FindCaseSensitively;
    }

    else {
        sensitivity = Qt::CaseInsensitive;
        this->flags &= ~QTextDocument::FindCaseSensitively;
    }
}

bool FindAndReplace::_find(QString exp, QTextDocument::FindFlags flags) {
    return editor->find( exp, flags );
}

bool FindAndReplace::_findRegEx(QString exp, QTextDocument::FindFlags flags) {
    return editor->find( QRegExp(exp, sensitivity), flags );
}

QTextCursor FindAndReplace::_find(QString exp, QTextCursor* cursor) {
    return this->editor->document()->find(exp, *cursor, flags);
}

QTextCursor FindAndReplace::_findRegEx(QString exp, QTextCursor* cursor) {
    return this->editor->document()->find( QRegExp(exp, sensitivity), *cursor, flags );
}

bool FindAndReplace::find(QString exp, bool backward) {
    QTextDocument::FindFlags flags = this->flags;
    bool found;

    if(backward)
        flags |= QTextDocument::FindBackward;

    found = (this->*findPtr)(exp, flags);

    if(!found) {
        if(!editor->document()->isEmpty()) {
            QMessageBox::warning(nullptr, "Warning", "No match found");
            return false;
        }
    }

    return true;
}

bool FindAndReplace::findall(QString exp) {
    if(editor->document()->isEmpty())
        return false;

    QTextCursor cursor;
    QTextCharFormat findExp_format;
    findExp_format.setBackground(findExp_highlightColor);

    // move it the beginning of the document
    cursor = editor->textCursor();
    cursor.movePosition(QTextCursor::Start);

    while (!(cursor = (this->*findAllPtr)(exp, &cursor)).isNull()) {
        // save current search result properties for clearing it
        // after finishing searching
        searchResultList.append( { cursor.charFormat().background(),
                                   cursor } );

        // highlight it
        cursor.mergeCharFormat(findExp_format);
    }

    if(searchResultList.length() == 0) {
        QMessageBox::warning(nullptr, "Warning", "No match found");
        return false;
    }

    return true;
}

void FindAndReplace::replace(QString exp, QString replacement) {
    // replacement is done by finding and highlighting `exp` first
    // then replace it
    // TODO: need better implementation
    if(editor->textCursor().hasSelection())
        editor->insertPlainText(replacement);

    editor->find(exp);
}

void FindAndReplace::replaceall(QString exp, QString replacement) {
    // mark all change in replacement as once block to be undo at one time
    QTextCursor cursor = editor->textCursor();
    cursor.beginEditBlock();

    while(editor->find(exp)) {
//        if(editor->textCursor().hasSelection())
        editor->insertPlainText(replacement);
    }

    cursor.endEditBlock();
}

void FindAndReplace::clearSearchHighlight() {
    QTextCharFormat format;

    if(searchResultList.isEmpty())
        return;

    foreach (FindAndReplace::SearchResultProperties searchFoundProperties, searchResultList) {
        format.setBackground(searchFoundProperties.bg_color);
        searchFoundProperties.cursor.mergeCharFormat(format);
    }

    searchResultList.clear();
}

FindAndReplace::~FindAndReplace() {

}
