#include "tabmanagement.h"
#include <QFileDialog>
#include <QDir>
#include <QShowEvent>
#include "commandslotdb.h"

TabManagement::TabManagement(QWidget* parent)
    : QTabWidget(parent) {
    // event on tab close
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    // event on current tab changed (swapped to another tab)
    connect( this,
             &QTabWidget::currentChanged,
             [=](int tabIndex){ emit tabNameChanged(this->tabText(tabIndex)); } );
}

void TabManagement::showEvent(QShowEvent* event) {
    Q_UNUSED(event);

    if(event->type() == QShowEvent::Show) {
        // register slots on this tabwidget shown
        CommandSlotDB* commandSlotDB = CommandSlotDB::getInstance();
        commandSlotDB->registerSlot( "new", this, _SLOT(newTab()) );
        commandSlotDB->registerSlot( "open", this, _SLOT(openTab()) );
        commandSlotDB->registerSlot( "save",
                                     this,
                                     _SLOT(qobject_cast<DocumentWidget *>
                                           (currentWidget())->document_save())
                                   );
        commandSlotDB->registerSlot( "save_as",
                                     this,
                                     _SLOT(qobject_cast<DocumentWidget *>
                                           (currentWidget())->document_saveAs())
                                   );
        commandSlotDB->registerSlot( "close", this, _SLOT(closeTab()) );
    }
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
    return QFileDialog::getOpenFileName( nullptr,
                                         tr("Open file"),
                                         QDir::homePath(),
                                         tr("All Files (*)") );
}

bool TabManagement::closeTab(int index, bool createNewTabIfLastOneClosed) {
    if(index < 0)
        return false;

    DocumentWidget* widget = static_cast<DocumentWidget*>(this->widget(index));

    // close tab
    if(widget->document_close())
        this->removeTab(index);

    else
        return false;

    // create new tab if the last tab closed
    if(createNewTabIfLastOneClosed && (this->count() == 0))
        newTab();

    if(this->count() != 0)
        // always focus the next available widget
        this->currentWidget()->setFocus();

    return true;
}

bool TabManagement::closeTab(int index) {
    return this->closeTab(index, true);
}

bool TabManagement::closeTab() {
    return this->closeTab(this->currentIndex());
}

bool TabManagement::closeAllTabs() {
    for(int iii = 0; iii < this->count(); ++iii) {
        if( qobject_cast<DocumentWidget*>(this->widget(iii))->isModified() )
            return this->closeTab(iii);
    }

    for(int iii = 0; iii < this->count(); ++iii) {
        this->closeTab();
    }

    return true;
}

QString TabManagement::getCurrentTabText() {
    return this->tabText(this->currentIndex());
}

TabManagement::~TabManagement() {
    // FIXME: we need to keep mainwindow open until finish that part
    // close every tab first
//    for(int iii = 0; iii < this->count(); ++iii) {
//        this->closeTab(iii, false);
//    }
}
