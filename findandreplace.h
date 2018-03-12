#ifndef FINDANDREPLACE_H
#define FINDANDREPLACE_H

#include <QWidget>
#include <QTextCursor>
#include <QList>
#include <QPlainTextEdit>
#include <QRegExp>

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
    QTextDocument::FindFlags flags = 0;
    QRegExp::PatternSyntax regExpPattern = QRegExp::FixedString;
    Qt::CaseSensitivity sensitivity = Qt::CaseInsensitive;

    // `find` variables
    QTextCursor findExp_cursor;
    bool (FindAndReplace::* findPtr)(QString, QTextDocument::FindFlags) = &FindAndReplace::_find;

    // `findall` variables
    QTextCursor (FindAndReplace::* findAllPtr)(QString, QTextCursor*) = &FindAndReplace::_find;
    const Qt::GlobalColor findExp_highlightColor = Qt::yellow;
    QList<SearchResultProperties> searchResultList;

private:
    bool _find(QString exp, QTextDocument::FindFlags flags);
    QTextCursor _find(QString exp, QTextCursor* cursor);
    bool _findRegEx(QString exp, QTextDocument::FindFlags flags);
    QTextCursor _findRegEx(QString exp, QTextCursor* cursor);

public slots:
    void setRgExMode(bool enable);
    void setCaseSensitive(bool enable);
    bool find(QString exp, bool backward=false);
    bool findall(QString exp);
    void replace(QString exp, QString replacement);
    void replaceall(QString exp, QString replacement);
    void clearSearchHighlight();
};

#endif // FINDANDREPLACE_H
