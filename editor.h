#ifndef EDITOR_H
#define EDITOR_H

#include <QTextEdit>
#include <QWidget>

class Editor : public QTextEdit
{
public:
    Editor(QWidget* parent);
    ~Editor();
};

#endif // EDITOR_H
