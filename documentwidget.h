#ifndef DOCUMENTWIDGET_H
#define DOCUMENTWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QGridLayout>
#include <QFile>
#include <QTextStream>
#include <QShortcut>

class DocumentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DocumentWidget(QWidget *parent = nullptr);
    QString getDocumentName();
    bool isModified();
    ~DocumentWidget();

private:
    QTextEdit* textEdit_widget;
    QGridLayout* layout;
    QFile* file = NULL;
    QTextStream* file_textStream = NULL;
    const QFile::OpenMode file_openModeFlags = QFile::ReadWrite | QFile::Text;
    QString fileName = "";

private:
    void configure_layout();
    void configure_textEdit_widget();
    static QString calculateDocumentName(QString path);
    bool document_save(bool saveAsNewFile);

signals:
    void documentModified(bool changed);
    void documentNameChanged(QString newName);

public slots:
    bool document_new();
    bool document_open(QString path);
    bool document_save();
    bool document_saveAs();
    bool document_close();
    bool close();
};

#endif // DOCUMENTWIDGET_H
