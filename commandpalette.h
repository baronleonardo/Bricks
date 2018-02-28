#ifndef COMMANDPALETTE_H
#define COMMANDPALETTE_H

#include <QLineEdit>
#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QTimer>
#include "commandfilter.h"

class CommandPalette : public QLineEdit
{
    Q_OBJECT

public:
    CommandPalette(QWidget* parent = nullptr);
    ~CommandPalette();

private:
    QTreeView* popup = nullptr;
    QStandardItemModel* model = nullptr;
    CommandFilter* filter = nullptr;
    QTimer timer;
    const int timeForSuggetionShow = 250;
    int commandsCount = 0;
    const int commandsTreeViewColCount = 2;

private:
    void addCommand(QString cmd, QString shortcut);
    void removeCommand(int rowIndex);
    bool eventFilter(QObject *obj, QEvent *ev);

private slots:
    void doneCompletion();
    void showCompletion();
    void autoSuggest();
};

#endif // COMMANDPALETTE_H
