#ifndef EDITOR_H
#define EDITOR_H

#include <QPlainTextEdit>
#include <QWidget>
#include <QTextStream>

class Editor : public QPlainTextEdit
{
    Q_OBJECT

public:
    Editor(QWidget* parent = NULL);
    bool write(QTextStream* file_stream);
    void setModified(bool modified);
    bool isModified();
    bool isEmpty();
    ~Editor();
};

#endif // EDITOR_H
