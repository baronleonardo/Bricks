#ifndef FINDANDREPLACE_H
#define FINDANDREPLACE_H

#include <QWidget>
#include <QTextCursor>
#include <QList>
#include <QPlainTextEdit>

class FindAndReplace : public QWidget
{
    Q_OBJECT

public:
    explicit FindAndReplace(QPlainTextEdit* parent);
    ~FindAndReplace();

private:
    struct SearchResultProperties {
        QBrush bg_color;
        QTextCursor cursor;
    };

private:
    QPlainTextEdit* editor;

    // `find` variables
    QTextCursor findExp_cursor;
    bool canContinueSearchingFromTop = true;
    bool foundSearchMatch = false;

    // `findall` variables
    const Qt::GlobalColor findExp_highlightColor = Qt::yellow;
    QList<SearchResultProperties> searchResultList;

public:
    // TODO: need to search by regex too
    // find here will start searching from the cursor position
    void find(QString exp);
    void findall(QString exp);
    void replace(QString exp, QString replacement);
    void replaceall(QString exp, QString replacement);
    void clearSearchHighlight();
};

#endif // FINDANDREPLACE_H
