#ifndef EDITOR_H
#define EDITOR_H

#include <QTextEdit>
#include <QWidget>
#include <QTextStream>
#include <QString>
#include <QFile>

class Editor : public QTextEdit
{
    Q_OBJECT

public:
    Editor(QWidget* parent = NULL);
    ~Editor();

private:
    QFile* file = NULL;
    QTextStream *stream = NULL;
    bool prepare_file(QString path);

public slots:
    void save();
    void open(QString path);
};

#endif // EDITOR_H
