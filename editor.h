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
    const QBrush findExp_highlightColor = Qt::yellow;
    bool prepare_file(QString path);
    void find(QString exp, bool firstMatchOnly);

public slots:
    void save();
    void open(QString path);
    // TODO: need to search by regex too
    // find her will start searching from the cursor position
    void find(QString exp);
    void findall(QString exp);
};

#endif // EDITOR_H
