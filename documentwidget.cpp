#include "tabwidget.h"
#include <QFileInfo>

TabWidget::TabWidget(QWidget *parent)
    : QWidget(parent) {
    textEdit_widget = new QTextEdit(this);
    this->configure_textEdit_widget();

    layout = new QGridLayout(this);
    this->configure_layout();

    this->setLayout(layout);
}

void TabWidget::configure_layout() {
    layout->addWidget(textEdit_widget);
    layout->setContentsMargins(0, 0, 0, 0);
}

void TabWidget::configure_textEdit_widget() {
    textEdit_widget->setFocusPolicy(Qt::StrongFocus);
}

QString TabWidget::getTabName(QString path) {
    QFileInfo fileInfo(path);
    return fileInfo.fileName();
}

TabWidget::~TabWidget() {
    delete textEdit_widget;
}
