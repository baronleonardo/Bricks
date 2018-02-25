#ifndef TABMANAGEMENT_H
#define TABMANAGEMENT_H

#include <QTabWidget>
#include "documentwidget.h"

class TabManagement : public QTabWidget
{
    Q_OBJECT

public:
    TabManagement(QWidget* parent = NULL);
    QString getCurrentTabText();
    ~TabManagement();

private:
    const QString defaultTabName = "Untitled Document";
    QString openFileDialog();
    void closeTab(int index, bool createNewTabIfLastOneClosed);

signals:
    void tabNameChanged(QString name);

private slots:
    void onTabDocumentModification(bool modified);

public slots:
    void newTab();
    void openTab(QString path);
    void openTab();
    void closeTab(int index);
};

#endif // TABMANAGEMENT_H
