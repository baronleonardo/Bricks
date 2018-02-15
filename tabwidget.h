#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QGridLayout>

class TabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = nullptr);
    static QString getTabName(QString path);
    ~TabWidget();

private:
    QTextEdit* textEdit_widget;
    QGridLayout* layout;
    void configure_layout();
    void configure_textEdit_widget();

signals:

public slots:
};

#endif // TABWIDGET_H
