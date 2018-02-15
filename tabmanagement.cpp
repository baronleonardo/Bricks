#include "tabmanagement.h"
#include <QFileDialog>
#include <QDir>

TabManagement::TabManagement(QWidget* parent)
    : QTabWidget(parent) {
    this->setFocusPolicy(Qt::ClickFocus);
    this->setDocumentMode(true);
}

void TabManagement::newTab() {
    int index = this->addTab(new TabWidget(this), this->defaultTabName);
    this->setCurrentIndex(index);
}

void TabManagement::openTab() {
    QString filePath = this->openFileDialog();
    int index = this->addTab(new TabWidget(this), TabWidget::getTabName(filePath));
    this->setCurrentIndex(index);
}

QString TabManagement::openFileDialog() {
    return QFileDialog::getOpenFileName( this,
                                         tr("Open file"),
                                         QDir::homePath(),
                                         tr("All Files (*)") );
}

TabManagement::~TabManagement() {
    delete tabWidget;
}
