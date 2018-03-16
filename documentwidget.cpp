#include "documentwidget.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QTextDocument>
#include "commandslotdb.h"

DocumentWidget::DocumentWidget(QWidget *parent)
    : QWidget(parent) {
    // delete this widget on close
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    // create new editor
    editor = new Editor(this);
    this->configure_textEdit_widget();

    // create new find and replace widget
    findAndReplace = new FindAndReplace(editor);
    this->configure_findAndReplaceWidget();

    // create layout
    layout = new QGridLayout(this);
    this->configure_layout();

    // set this layout as the default one
    this->setLayout(layout);

    // send signal on document modification
    connect( editor,
             &Editor::modificationChanged,
             [=](bool changed){emit this->documentModified(changed);});
}

void DocumentWidget::configure_layout() {
    layout->addWidget(editor);
    layout->addWidget(findAndReplace);
    layout->setContentsMargins(0, 0, 0, 0);
}

void DocumentWidget::configure_textEdit_widget() {
    // focus textedit widget if this document widget gets focused
    this->setFocusProxy(editor);
    editor->setLineWrapMode(Editor::NoWrap);
}

void DocumentWidget::configure_findAndReplaceWidget() {
    findAndReplace->close();
    CommandSlotDB::getInstance()->registerSlot( "find_and_replace",
                                                this,
                                                _SLOT(findAndReplace->show()) );
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
        this->editor->write(file_textStream);
        this->editor->document()->setModified(false);

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
    if( editor->document()->isEmpty() )
        return false;

    // if new file
    else if(this->file_textStream == NULL || saveAsNewFile) {
        // create new document
        if(!document_new())
            return false;
    }

    *file_textStream << editor->toPlainText();
    file_textStream->flush();

    // mark this document as not modified after saving
    this->editor->document()->setModified(false);

    return true;
}

bool DocumentWidget::document_save() {
    if(this->editor->isModified())
        return this->document_save(false);

    return false;
}

bool DocumentWidget::document_saveAs() {
    return this->document_save(true);
}

bool DocumentWidget::document_close() {
    if(editor->isModified()) {
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
    return editor->isModified();
}

DocumentWidget::~DocumentWidget() {
    delete editor;

    if(file_textStream != NULL)
        delete file_textStream;

    if(file != NULL) {
        file->close();
        delete file;
    }
}
