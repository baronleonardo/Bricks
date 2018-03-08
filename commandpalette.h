#ifndef COMMANDPALETTE_H
#define COMMANDPALETTE_H

#include <QLineEdit>
#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QTimer>
#include <QAction>
#include "commandfilter.h"

class CommandPalette : public QLineEdit
{
    Q_OBJECT

public:
    CommandPalette(QWidget* parent = nullptr);
    void addCommand(QString cmd, QString shortcut, bool addEnabled = true);
    void removeCommand(int rowIndex);
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
    bool eventFilter(QObject *obj, QEvent *ev);

public:
    void setVisible(bool visible);
    void setHidden(bool hidden);

protected:
    void focusOutEvent(QFocusEvent* event);

signals:
    void commandactivated(QString cmd);
    void focusLost();

private slots:
    void doneCompletion();
    void showCompletion();
    void autoSuggest();

public slots:
    bool enableCommand(QString cmd);
    bool disableCommand(QString cmd);
};

#endif // COMMANDPALETTE_H
