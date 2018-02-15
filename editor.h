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
    bool write(QTextStream* file_stream);
    ~Editor();

private:
    const QBrush findExp_highlightColor = Qt::yellow;
    void find(QString exp, bool firstMatchOnly);
    int currentSearchIndex = -1;

public slots:
    // TODO: need to search by regex too
    // find her will start searching from the cursor position
    void find(QString exp);
    void findall(QString exp);
};

#endif // EDITOR_H
