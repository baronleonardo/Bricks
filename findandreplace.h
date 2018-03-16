#ifndef FINDANDREPLACE_H
#define FINDANDREPLACE_H

#include <QWidget>
#include <QTextCursor>
#include <QList>
#include <QPlainTextEdit>
#include <QRegExp>
#include "ui_find_and_replace.h"

class FindAndReplace : public QWidget
{
    Q_OBJECT

public:
    explicit FindAndReplace(QPlainTextEdit *parent);
    ~FindAndReplace();

protected:
    void showEvent(QShowEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

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
    bool canContinueSearchingFromTop = false;

    Ui::FindAndReplaceUI* findAndReplaceUi;

    // `find` variables
    QTextCursor findExp_cursor;
//    bool (FindAndReplace::* findPtr)(QString, QTextDocument::FindFlags) = &FindAndReplace::_find;
    QTextCursor (FindAndReplace::* findPtr)(QString,
                                            QTextCursor*,
                                            QTextDocument::FindFlags) = &FindAndReplace::_find;

    // `findall` variables
    const Qt::GlobalColor findExp_highlightColor = Qt::yellow;
    QList<SearchResultProperties> searchResultList;

private:
//    bool _find(QString exp, QTextDocument::FindFlags flags);
    QTextCursor _find(QString exp, QTextCursor* cursor, QTextDocument::FindFlags flags);
//    bool _findRegEx(QString exp, QTextDocument::FindFlags flags);
    QTextCursor _findRegEx(QString exp, QTextCursor* cursor, QTextDocument::FindFlags flags);

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
