#include "editor.h"

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

Editor::~Editor() {
}