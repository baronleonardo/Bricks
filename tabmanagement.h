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
    const QString defaultTabName = tr("Untitled Document");
    QString openFileDialog();
    bool closeTab(int index, bool createNewTabIfLastOneClosed);

protected:
    void showEvent(QShowEvent* event);

signals:
    void tabNameChanged(QString name);

private slots:
    void onTabDocumentModification(bool modified);

public slots:
    void newTab();
    void openTab(QString path);
    void openTab();
    bool closeTab(int index);
    bool closeTab();
    bool closeAllTabs();
};

#endif // TABMANAGEMENT_H
