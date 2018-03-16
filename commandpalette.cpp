#include "commandpalette.h"
#include <QHeaderView>
#include <QShortcut>
#include <QApplication>
#include <QKeyEvent>

CommandPalette::CommandPalette(QWidget* parent) : QLineEdit(parent) {
    this->popup = new QTreeView(this);
    popup->setWindowFlags(Qt::Popup);
    popup->setFocusPolicy(Qt::NoFocus);
    popup->setFocusProxy(parent);
    popup->setMouseTracking(true);
    popup->setUniformRowHeights(true);
    popup->setRootIsDecorated(false);
    popup->setEditTriggers(QTreeView::NoEditTriggers);
    popup->setSelectionBehavior(QTreeView::SelectRows);
    popup->setSelectionMode(QTreeView::SingleSelection);
    popup->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    popup->header()->hide();

    popup->installEventFilter(this);

    connect(popup, SIGNAL(activated(QModelIndex)),
                SLOT(doneCompletion()));

    timer.setSingleShot(true);
    timer.setInterval(timeForSuggetionShow);
    connect(&timer, SIGNAL(timeout()), SLOT(autoSuggest()));
    connect(this, SIGNAL(textEdited(QString)), &timer, SLOT(start()));

    connect( new QShortcut(QKeySequence(Qt::Key_Down), this),
             &QShortcut::activated,
             this,
             &CommandPalette::showCompletion);

    // if the command palette hold an old triggered command
    // u can just trigger it without opening popup
    connect( new QShortcut(QKeySequence(Qt::Key_Return), this),
             &QShortcut::activated,
             this,
             [=](){ if(this->text() != "") commandactivated(this->text()); });

    filter = new CommandFilter;
    popup->setModel(filter);
    model = new QStandardItemModel(commandsCount, commandsTreeViewColCount, popup);
    filter->setSourceModel(model);
}

void CommandPalette::addCommand(QString cmd, QString shortcut, bool addEnabled) {
    QFont font;
    font.setPointSize(QApplication::font().pointSize() - 2);
    font.setItalic(true);
    int rowIndex = model->rowCount();

    model->insertRow(rowIndex);

    model->setData(model->index(rowIndex, 0), cmd);
    model->setData(model->index(rowIndex, 1), shortcut);

    QStandardItem* col0 = model->item(rowIndex, 0);
    QStandardItem* col1 = model->item(rowIndex, 1);

    col1->setFont(font);

    if(!addEnabled) {
        col0->setEnabled(false);
        col0->setSelectable(false);
        col1->setEnabled(false);
        col1->setSelectable(false);
    }

    commandsCount++;
}

void CommandPalette::removeCommand(int rowIndex) {
    model->removeRow(rowIndex);
    commandsCount--;
}

bool CommandPalette::enableCommand(QString cmd) {
    QList<QStandardItem*> items = model->findItems(cmd, Qt::MatchContains);
    if(items.length() == 0)
        return false;

    int rowIndex = items[0]->row();

    model->item(rowIndex, 0)->setEnabled(true);
    model->item(rowIndex, 0)->setSelectable(true);
    model->item(rowIndex, 1)->setEnabled(true);
    model->item(rowIndex, 1)->setSelectable(true);

    return true;
}

bool CommandPalette::disableCommand(QString cmd) {
    QList<QStandardItem*> items = model->findItems(cmd, Qt::MatchWildcard);
    if(items.length() == 0)
        return false;

    int rowIndex = items[0]->row();

    model->item(rowIndex, 0)->setEnabled(false);
    model->item(rowIndex, 0)->setSelectable(false);
    model->item(rowIndex, 1)->setEnabled(false);
    model->item(rowIndex, 1)->setSelectable(false);

    return true;
}

bool CommandPalette::eventFilter(QObject *obj, QEvent *ev) {
    if(obj != popup)
        return false;

    if (ev->type() == QEvent::MouseButtonPress) {
            popup->hide();
            this->setFocus();
            return true;
        }

    if (ev->type() == QEvent::KeyPress) {
        bool consumed = false;
        int key = static_cast<QKeyEvent*>(ev)->key();
        switch (key) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            doneCompletion();
            consumed = true;
            break;

        case Qt::Key_Escape:
            this->setFocus();
            popup->hide();
            consumed = true;
            break;

        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Home:
        case Qt::Key_End:
        case Qt::Key_PageUp:
        case Qt::Key_PageDown:
            break;

        default:
            this->setFocus();
            this->event(ev);
//            popup->hide();
            break;
        }

        return consumed;
    }

    return false;
}

void CommandPalette::focusOutEvent(QFocusEvent* event) {
    Q_UNUSED(event);
    if(!popup->hasFocus())
        emit(this->focusLost());
}

void CommandPalette::showCompletion() {
    popup->move( this->mapToGlobal(QPoint(0, this->height())) );
    popup->setColumnWidth(0, this->width() *  2/3);
    popup->setFocus();
    // select first row by default
    popup->setCurrentIndex(popup->model()->index(0,0));
    popup->show();
    popup->resize(this->width(), popup->height());
}

void CommandPalette::doneCompletion() {
    timer.stop();
    popup->hide();
    this->setFocus();

    QModelIndex item = popup->currentIndex();

    if (item.isValid()) {
        this->setText(item.data().toString());
        QMetaObject::invokeMethod(this, "returnPressed");

        emit(commandactivated(item.data().toString()));
    }
}

void CommandPalette::autoSuggest() {
    if(this->text() != "") {
        filter->setFilterRegExp( QRegExp( this->text(),
                                          Qt::CaseInsensitive,
                                          QRegExp::FixedString )
                               );
        showCompletion();
    }

    else
        filter->setFilterRegExp("");
}

void CommandPalette::setVisible(bool visible) {
    QLineEdit::setVisible(visible);
    this->setFocus();
}

void CommandPalette::setHidden(bool hidden) {
    QLineEdit::setHidden(hidden);

    if(!hidden)
        this->setFocus();
}

CommandPalette::~CommandPalette() {
    delete filter;
    delete model;
    delete popup;
}
