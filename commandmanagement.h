#ifndef COMMANDMANAGEMENT_H
#define COMMANDMANAGEMENT_H

#include <QObject>
#include <QApplication>
#include "commandpalette.h"
#include "commandslotdb.h"

class CommandPaletteCommandManagement;

class CommandManagement : public QObject
{
    Q_OBJECT

public:
    explicit CommandManagement( CommandPalette *commandpalette,
                                QObject* parent = nullptr );
    void createCommandsDB();
    ~CommandManagement();

private:
    const QString iniFilePath = QApplication::applicationFilePath() + ".ini";
    CommandSlotDB* commandSlotDB;
    CommandPaletteCommandManagement *commandPaletteCommandManagement;
    QString commandPaletteShortcut;

private:
    void prepareCommandsSystem();

public slots:
    void enableCommand(QString cmd);
    void disableCommand(QString cmd);
    void onCommandActivation(QString cmd);
};

class CommandPaletteCommandManagement: public QObject {
    Q_OBJECT

public:
    CommandPaletteCommandManagement( CommandPalette *commandPalette,
                                     QWidget* parent = nullptr );
    ~CommandPaletteCommandManagement();

private:
    CommandPalette* commandPalette = nullptr;

private:
    QString prepareCommand(QString cmd, bool toCommandPalette);

public:
    void addCommand(QString cmd, QString shortcut);
    bool activateCommand(QString cmd);
    bool deactivateCommand(QString cmd);

signals:
    void commandActivated(QString cmd);

public slots:
    void onCommandActivation(QString cmd);
    void showCommandPalette();
    void hideCommandPalette();
    void toggleCommandPalette();
};

#endif // COMMANDMANAGEMENT_H
