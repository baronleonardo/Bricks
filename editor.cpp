#include "editor.h"

Editor::Editor(QWidget *parent) :
    QTextEdit(parent) {
}

void Editor::save() {
    if(!stream) return;

    // TODO: optimization
    // delete the old text first
    file->resize(0);
    *stream << this->toPlainText();
    stream->flush();
}

bool Editor::prepare_file(QString path) {
    // if there was an old opened file
    if(file) delete file;
    if(stream) delete stream;

    this->file = new QFile(path);
    if( !this->file->open(QIODevice::ReadWrite | QIODevice::Text) )
        return false;

    stream = new QTextStream(file);

    return true;
}

void Editor::open(QString path) {
    if(!prepare_file(path))
        return;

    // clear textedit widget if there is old text
    if(!this->toPlainText().isEmpty())
        this->clear();

    this->setText(this->stream->readAll());
}

void Editor::find(QString exp, bool firstMatchOnly) {
    // get current cursor position
    QTextCursor cursor = this->textCursor();
    // find from that position next occurance of exp
    int index = this->toPlainText().indexOf(exp, cursor.position());

    QTextCharFormat format = QTextCharFormat();
    format.setBackground(findExp_highlightColor);

    while(index != -1) {
        cursor.setPosition(index);
        cursor.setPosition(index + exp.length(), QTextCursor::KeepAnchor);
        cursor.mergeCharFormat(format);

        if(firstMatchOnly) return;

        index = this->toPlainText().indexOf(exp, index + exp.length());
    }
}

void Editor::find(QString exp) {
    this->find(exp, true);
}

void Editor::findall(QString exp) {
    this->find(exp, false);
}

Editor::~Editor() {
    if(file) delete file;
    if(stream) delete stream;
}
