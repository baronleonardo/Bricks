#include "documentwidget.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QTextDocument>
#include <functional>
#include "commandslotdb.h"

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

    // send signal on document modification
    connect( textEdit_widget->document(),
             &QTextDocument::modificationChanged,
             [=](bool changed){emit this->documentModified(changed);});
}

void DocumentWidget::configure_layout() {
    layout->addWidget(textEdit_widget);
    layout->setContentsMargins(0, 0, 0, 0);
}

void DocumentWidget::configure_textEdit_widget() {
    // focus textedit widget if this document widget gets focused
    this->setFocusProxy(textEdit_widget);
    textEdit_widget->setLineWrapMode(QTextEdit::NoWrap);
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

        this->fileName = this->calculateDocumentName(path);
        // send signal that document name changed
        emit(this->documentNameChanged(this->fileName));

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
    this->fileName = this->calculateDocumentName(path);

    if(file->open(this->file_openModeFlags)) {
        file_textStream = new QTextStream(file);

        // send signal that document name changed
        emit(this->documentNameChanged(this->fileName));

        // TODO: need optimizations
        // read data from the file steam and write it to textEdit
        this->textEdit_widget->setText(file_textStream->readAll());
        this->textEdit_widget->document()->setModified(false);

        return true;
    }

    else {
        delete file;
        file = NULL;
        return false;
    }
}

bool DocumentWidget::document_save(bool saveAsNewFile) {
    // TODO: check if failed to save
    if( textEdit_widget->document()->isEmpty() )
        return false;

    // if new file
    else if(this->file_textStream == NULL || saveAsNewFile) {
        // create new document
        if(!document_new())
            return false;
    }

    *file_textStream << textEdit_widget->toPlainText();
    file_textStream->flush();

    // mark this document as not modified after saving
    this->textEdit_widget->document()->setModified(false);

    return true;
}

bool DocumentWidget::document_save() {
    if(this->textEdit_widget->document()->isModified())
        return this->document_save(false);

    return false;
}

bool DocumentWidget::document_saveAs() {
    return this->document_save(true);
}

bool DocumentWidget::document_close() {
    if(textEdit_widget->document()->isModified()) {
        int buttonIndex = QMessageBox::warning( nullptr,
                                                "Close Document",
                                                "Save changes ?",
                                                "Yes",
                                                "No",
                                                "Cancel",
                                                2 );

        if( buttonIndex == 0 )
            return this->document_save();
        else if( buttonIndex == 2 )
            return false;
    }

    return true;
}

bool DocumentWidget::close() {
    if(document_close())
        return QWidget::close();

    return false;
}

QString DocumentWidget::calculateDocumentName(QString path) {
    QFileInfo fileInfo(path);
    return fileInfo.fileName();
}

QString DocumentWidget::getDocumentName() {
    return this->fileName;
}

bool DocumentWidget::isModified() {
    return textEdit_widget->document()->isModified();
}

DocumentWidget::~DocumentWidget() {
    delete textEdit_widget;

    if(file_textStream != NULL)
        delete file_textStream;

    if(file != NULL) {
        file->close();
        delete file;
    }
}
