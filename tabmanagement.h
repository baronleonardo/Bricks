#ifndef TABMANAGEMENT_H
#define TABMANAGEMENT_H

#include <QTabWidget>
#include <QWidget>

class TabManagement : public QTabWidget
{
public:
    TabManagement(QWidget* parent = NULL);
    ~TabManagement();
};

#endif // TABMANAGEMENT_H
