#include "editor.h"

Editor::Editor(QWidget *parent) :
    QTextEdit(parent) {
    this->open("/tmp/main.cpp");
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

Editor::~Editor() {
    if(file) delete file;
    if(stream) delete stream;
}
