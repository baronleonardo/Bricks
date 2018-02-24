#include "documentwidget.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

DocumentWidget::DocumentWidget(QWidget *parent)
    : QWidget(parent) {
    // delete this widget on close
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    // create text edit widget
    textEdit_widget = new QTextEdit(this);
    this->configure_textEdit_widget();

    // create layout
    layout = new QGridLayout(this);
    this->configure_layout();

    // set this layout as the default one
    this->setLayout(layout);

    // create and configure actions
    this->configure_shortcuts();

    // send signal on document modification
    connect( textEdit_widget,
             &QTextEdit::textChanged,
             [=](){emit this->documentStateChanged(Modified);} );
}

void DocumentWidget::configure_layout() {
    layout->addWidget(textEdit_widget);
    layout->setContentsMargins(0, 0, 0, 0);
}

void DocumentWidget::configure_textEdit_widget() {
    // focus textedit widget if this document widget gets focused
    this->setFocusProxy(textEdit_widget);
}

void DocumentWidget::configure_shortcuts() {
    // TODO: all shortcuts will be moved to one class

    // `save` file
    connect( new QShortcut(QKeySequence::Save, this),
             &QShortcut::activated,
             this,
             &DocumentWidget::document_save );

    // `save as` file
    connect( new QShortcut(QKeySequence("Ctrl+Shift+S"), this),
             &QShortcut::activated,
             this,
             &DocumentWidget::document_save );
}

bool DocumentWidget::document_new() {
    // open at the same path as the current file exists
    QString fileDialogOpenPath = file != NULL ? file->fileName() : QDir::homePath();
    QString path = QFileDialog::getSaveFileName(this, "Save as", fileDialogOpenPath);

    if(path == "")
        return false;

    file = new QFile(path);

    if(file->open(this->file_openModeFlags)) {
        file_textStream = new QTextStream(file);
        return true;
    }

    else {
        delete file;
        file = NULL;
        return false;
    }
}

bool DocumentWidget::document_open(QString path) {
    file = new QFile(path);

    if(file->open(this->file_openModeFlags)) {
        file_textStream = new QTextStream(file);
        // TODO: need optimizations
        // read data from the file steam and write it to textEdit
        this->textEdit_widget->setText(file_textStream->readAll());
        return true;
    }

    else {
        delete file;
        file = NULL;
        return false;
    }
}

bool DocumentWidget::document_save() {
    // TODO: check if failed to save
    if( textEdit_widget->document()->isEmpty() )
        return false;

    // if new file
    else if(this->file_textStream == NULL) {
        // create new document
        if(!document_new())
            return false;
    }

    *file_textStream << textEdit_widget->toPlainText();
    file_textStream->flush();

    // mark this document as not modified after saving
    this->textEdit_widget->document()->setModified(false);

    // send signal that this document saved
    emit(this->documentStateChanged(Saved));

    return true;
}

bool DocumentWidget::close() {
    if(textEdit_widget->document()->isModified()) {
        int buttonIndex = QMessageBox::warning( this,
                                                "Close Document",
                                                "Save changes ?",
                                                "Yes",
                                                "No",
                                                "Cancel",
                                                2 );

        if( buttonIndex == 0 )
            this->document_save();
        else if( buttonIndex == 2 )
            return false;
    }

    // call the widget closs function
    return this->QWidget::close();
}

QString DocumentWidget::calculateDocumentName(QString path) {
    QFileInfo fileInfo(path);
    return fileInfo.fileName();
}

QString DocumentWidget::getDocumentName() {
    return this->textEdit_widget->documentTitle();
}

DocumentWidget::~DocumentWidget() {
    delete textEdit_widget;
    if(file != NULL) delete file;
    if(file_textStream != NULL) delete file_textStream;
}
