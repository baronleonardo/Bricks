#include "editor.h"

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

void Editor::find(QString exp, bool firstMatchOnly) {

    if(this->document()->isEmpty())
        return;

    // get current cursor position
    QTextCursor cursor;

    if(firstMatchOnly) {
        if(this->currentSearchIndex == -1)
            cursor = this->textCursor();
    }

    else
        cursor = this->cursorForPosition(0);

    // find from that position next occurance of exp
    this->currentSearchIndex =
            this->toPlainText().indexOf( exp, cursor.position() );

    QTextCharFormat format = QTextCharFormat();
    format.setBackground(findExp_highlightColor);

    while(this->currentSearchIndex != -1) {
        // mark the matched exp
        cursor.setPosition(this->currentSearchIndex);
        cursor.setPosition( this->currentSearchIndex + exp.length(),
                            QTextCursor::KeepAnchor );
        // highlight it
        cursor.mergeCharFormat(format);

        // update the search index
        this->currentSearchIndex =
                this->toPlainText().indexOf( exp, this->currentSearchIndex + exp.length() );

        if(firstMatchOnly) return;
    }
}

void Editor::find(QString exp) {
    this->find(exp, true);
}

void Editor::findall(QString exp) {
    this->find(exp, false);
}

Editor::~Editor() {
}
