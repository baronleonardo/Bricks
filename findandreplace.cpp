#include "findandreplace.h"
#include <QMessageBox>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>

FindAndReplace::FindAndReplace(QPlainTextEdit *parent) :
    QWidget(parent),
    findAndReplaceUi(new Ui::FindAndReplaceUI) {
    this->editor = parent;
    findAndReplaceUi->setupUi(this);

    // find next
    connect( findAndReplaceUi->find_NextBtn,
             &QPushButton::clicked,
             this,
             [=](){
                      if( !find(findAndReplaceUi->find_LineEdit->text()) )
                          QMessageBox::warning(nullptr, "Warning", "No match found");
                  } );
    // find previous
    connect( findAndReplaceUi->find_PreviousBtn,
             &QPushButton::clicked,
             this,
             [=](){
                      if( !find(findAndReplaceUi->find_LineEdit->text(), true) )
                          QMessageBox::warning(nullptr, "Warning", "No match found");
                  } );
    // find all
    connect( findAndReplaceUi->findAll_Btn,
             &QPushButton::clicked,
             this,
             [=](){
                      if( !findall(findAndReplaceUi->find_LineEdit->text()) )
                          QMessageBox::warning(nullptr, "Warning", "No match found");
                  } );
    // replace
    connect( findAndReplaceUi->replace_Btn,
             &QPushButton::clicked,
             this,
             [=](){ replace(findAndReplaceUi->find_LineEdit->text(),
                            findAndReplaceUi->replace_LineEdit->text()); } );
    // replace all
    connect( findAndReplaceUi->replaceAll_Btn,
             &QPushButton::clicked,
             this,
             [=](){ replaceall(findAndReplaceUi->find_LineEdit->text(),
                               findAndReplaceUi->replace_LineEdit->text()); } );
    // search mode - plain text, Regular expression
    connect( findAndReplaceUi->searchMode,
             QOverload<int>::of(&QComboBox::currentIndexChanged),
             this,
             [=](int value){ setRgExMode(value); } );
    // case sensitivity
    connect( findAndReplaceUi->caseSensitive_checkbox,
             &QCheckBox::clicked,
             this,
             [=](bool checked){ setCaseSensitive(checked); } );
}

void FindAndReplace::keyReleaseEvent(QKeyEvent* event) {
    if(event->key() == Qt::Key_Escape)
        this->close();

    QWidget::keyReleaseEvent(event);
}

void FindAndReplace::setRgExMode(bool enable) {
    if(enable)
        findPtr = &FindAndReplace::_findRegEx;

    else
        findPtr = &FindAndReplace::_find;
}

void FindAndReplace::setCaseSensitive(bool enable) {
    if(enable) {
        sensitivity = Qt::CaseSensitive;
        this->flags |= QTextDocument::FindCaseSensitively;
    }

    else {
        sensitivity = Qt::CaseInsensitive;
        this->flags &= ~QTextDocument::FindCaseSensitively;
    }
}

QTextCursor FindAndReplace::_find(QString exp,
                                  QTextCursor* cursor,
                                  QTextDocument::FindFlags flags) {
    return this->editor->document()->find(exp, *cursor, flags);
}

QTextCursor FindAndReplace::_findRegEx(QString exp,
                                       QTextCursor* cursor,
                                       QTextDocument::FindFlags flags) {
    return this->editor->document()->find( QRegExp(exp, sensitivity),
                                           *cursor,
                                           flags );
}

bool FindAndReplace::find(QString exp, bool backward) {
    QTextDocument::FindFlags flags = this->flags;

    if(editor->document()->isEmpty())
        return true;

    if(backward)
        flags |= QTextDocument::FindBackward;

    QTextCursor cursor = editor->textCursor();
    cursor = (this->*findPtr)(exp, &cursor, flags);

    if(!cursor.isNull()) {
        canContinueSearchingFromTop = true;
        editor->setTextCursor(cursor);
        return true;
    }

    else if(canContinueSearchingFromTop) {
        canContinueSearchingFromTop = false;
        backward ? editor->moveCursor(QTextCursor::End) :
                   editor->moveCursor(QTextCursor::Start);
        find(exp, backward);
        return true;
    }

    else
        return false;
}

bool FindAndReplace::findall(QString exp) {
    if(editor->document()->isEmpty())
        return true;

    if(!searchResultList.isEmpty())
        clearSearchHighlight();

    QTextCursor cursor;
    QTextCharFormat findExp_format;
    findExp_format.setBackground(findExp_highlightColor);

    // move it the beginning of the document
    cursor = editor->textCursor();
    cursor.movePosition(QTextCursor::Start);

    while (!(cursor = (this->*findPtr)(exp, &cursor, flags)).isNull()) {
        // save current search result properties for clearing it
        // after finishing searching
        searchResultList.append( { cursor.charFormat().background(),
                                   cursor } );

        // highlight it
        cursor.mergeCharFormat(findExp_format);
    }

    if(searchResultList.length() == 0)
        return false;

    return true;
}

void FindAndReplace::replace(QString exp, QString replacement) {
    // replacement is done by finding and highlighting `exp` first
    // then replace it
    if(editor->textCursor().hasSelection())
        editor->insertPlainText(replacement);

    this->find(exp);
}

void FindAndReplace::replaceall(QString exp, QString replacement) {
    QTextCursor old_cursor = editor->textCursor();
    editor->moveCursor(QTextCursor::Start);

    // mark all change in replacement as once block to be undo at one time
    old_cursor.beginEditBlock();

    while(editor->find(exp))
        editor->insertPlainText(replacement);

    old_cursor.endEditBlock();

    editor->setTextCursor(old_cursor);
}

void FindAndReplace::clearSearchHighlight() {
    QTextCharFormat format;

    if(searchResultList.isEmpty())
        return;

    foreach (FindAndReplace::SearchResultProperties searchFoundProperties, searchResultList) {
        format.setBackground(searchFoundProperties.bg_color);
        searchFoundProperties.cursor.mergeCharFormat(format);
    }

    searchResultList.clear();
}

void FindAndReplace::showEvent(QShowEvent* event) {
    if(event->type() == QShowEvent::Show)
        findAndReplaceUi->find_LineEdit->setFocus();
    QWidget::showEvent(event);
}

FindAndReplace::~FindAndReplace() {
    delete findAndReplaceUi;
}
