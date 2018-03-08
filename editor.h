#ifndef EDITOR_H
#define EDITOR_H

#include <QPlainTextEdit>
#include <QWidget>
#include <QTextStream>
#include "findandreplace.h"

class Editor : public QPlainTextEdit
{
    Q_OBJECT

public:
    Editor(QWidget* parent = NULL);
    ~Editor();

private:
    FindAndReplace* findAndReplace;

public:
    bool write(QTextStream* file_stream);
};

#endif // EDITOR_H
