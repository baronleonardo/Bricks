#ifndef EDITOR_H
#define EDITOR_H

#include <QPlainTextEdit>
#include <QWidget>
#include <QTextStream>
#include <QString>
#include <QTextCursor>

class Editor : public QPlainTextEdit
{
    Q_OBJECT

public:
    Editor(QWidget* parent = NULL);
    bool write(QTextStream* file_stream);
    ~Editor();

private:
    class SearchResultProperties {
        public: QBrush bg_color;
        public: QTextCursor cursor;
    };

private:
    // `find` variables
    QTextCursor findExp_cursor;
    bool canContinueSearchingFromTop = true;
    bool foundSearchMatch = false;

    // `findall` variables
    const QBrush findExp_highlightColor = Qt::yellow;
    QList<SearchResultProperties> searchResultList;

public slots:
    // TODO: need to search by regex too
    // find here will start searching from the cursor position
    void find();
    void find(QString exp);
    void findall(QString exp);
    void clearSearchHighlight();
};

#endif // EDITOR_H
