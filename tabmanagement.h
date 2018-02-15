#ifndef TABMANAGEMENT_H
#define TABMANAGEMENT_H

#include <QTabWidget>
#include "tabwidget.h"

class TabManagement : public QTabWidget
{
    Q_OBJECT

public:
    TabManagement(QWidget* parent = NULL);
    ~TabManagement();

private:
    TabWidget* tabWidget;
    const QString defaultTabName = "Untitled Document";
    QString openFileDialog();

public slots:
    void newTab();
    void openTab();
};

#endif // TABMANAGEMENT_H
