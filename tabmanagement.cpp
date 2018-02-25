#include "tabmanagement.h"
#include <QFileDialog>
#include <QDir>

TabManagement::TabManagement(QWidget* parent)
    : QTabWidget(parent) {

    // shortcut for new tab
    connect( new QShortcut(QKeySequence::New, this),
             &QShortcut::activated,
             this,
             &TabManagement::newTab );

    // shortcut for close current tab
    connect( new QShortcut(QKeySequence("Ctrl+W"), this),
             &QShortcut::activated,
             [=]() {this->closeTab(this->currentIndex());} );

    // event on tab close
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    // event on current tab changed (swapped to another tab)
    connect( this,
             &QTabWidget::currentChanged,
             [=](int tabIndex){ emit tabNameChanged(this->tabText(tabIndex)); } );
}

void TabManagement::newTab() {
    DocumentWidget* tabwidget = new DocumentWidget(this);
    int index = this->addTab(tabwidget, this->defaultTabName);
    this->setCurrentIndex(index);

    // change tab name on content of tabwidget changed (document modified)
    connect( tabwidget,
             &DocumentWidget::documentModified,
             this,
             &TabManagement::onTabDocumentModification );

    // change tab name if filename changed
    // emit signal for the change
    connect( tabwidget,
             &DocumentWidget::documentNameChanged,
             [=](QString newName) {
                this->setTabText(index, newName);
                emit tabNameChanged(newName);
             } );
}

void TabManagement::onTabDocumentModification(bool modified) {
    int currentIndex = this->currentIndex();
    QString documentName =
            static_cast<DocumentWidget *>(this->currentWidget())->getDocumentName();

    if(documentName == "")
        documentName = this->defaultTabName;

    if(modified)
        documentName = documentName + " *";

    this->setTabText( currentIndex, documentName );
    emit tabNameChanged(documentName);
}

void TabManagement::openTab(QString path) {
    // create a new tab first
    this->newTab();

    DocumentWidget* tabwidget = static_cast<DocumentWidget*>(this->currentWidget());
    tabwidget->document_open(path);
    // set tab text
    this->setTabText( this->currentIndex(), tabwidget->getDocumentName() );

    emit tabNameChanged(tabwidget->getDocumentName());
}

void TabManagement::openTab() {
    QString filePath = this->openFileDialog();
    if(filePath != "")
        this->openTab(filePath);
}

QString TabManagement::openFileDialog() {
    return QFileDialog::getOpenFileName( this,
                                         tr("Open file"),
                                         QDir::homePath(),
                                         tr("All Files (*)") );
}

void TabManagement::closeTab(int index, bool createNewTabIfLastOneClosed) {
    if(index < 0)
        return;

    DocumentWidget* widget = static_cast<DocumentWidget*>(this->widget(index));

    // close tab
    if(widget->close()) {
        delete widget;
        this->removeTab(index);
    }

    // create new tab if the last tab closed
    if(createNewTabIfLastOneClosed && (this->count() == 0))
        newTab();

    if(this->count() != 0)
        // always focus the next available widget
        this->currentWidget()->setFocus();
}

void TabManagement::closeTab(int index) {
    this->closeTab(index, true);
}

QString TabManagement::getCurrentTabText() {
    return this->tabText(this->currentIndex());
}

TabManagement::~TabManagement() {
    // FIXME: we need to keep mainwindow open until finish that part
    // close every tab first
    for(int iii = 0; iii < this->count(); ++iii) {
        this->closeTab(iii, false);
    }
}
