#include "commandmanagement.h"
#include <QSettings>
#include <QFile>
#include <QShortcut>
#include <QFileInfo>

CommandManagement::CommandManagement( CommandPalette* commandpalette,
                                      QObject *parent ) : QObject(parent) {
    commandPaletteCommandManagement =
            new CommandPaletteCommandManagement( commandpalette,
                                                 qobject_cast<QWidget*>(parent) );

    commandSlotDB = CommandSlotDB::getInstance();

    if(!QFileInfo::exists(iniFilePath))
        this->createCommandsDB();
    this->prepareCommandsSystem();

    // enable command in command palette
    connect( commandSlotDB,
             &CommandSlotDB::slotRegistered,
             this,
             &CommandManagement::enableCommand );

    // commandpalette command shortcut
    connect( new QShortcut(QKeySequence(commandPaletteShortcut), qobject_cast<QWidget*>(parent)),
             &QShortcut::activated,
             commandPaletteCommandManagement,
             &CommandPaletteCommandManagement::toggleCommandPalette );

    // commandpalette command activation
    connect( commandPaletteCommandManagement,
             &CommandPaletteCommandManagement::commandActivated,
             this,
             &CommandManagement::onCommandActivation );
}

void CommandManagement::createCommandsDB() {
    QFile src(":/ini/shortcuts.ini");
    src.open(QFile::ReadOnly);
    QFile dest(iniFilePath);
    dest.open(QFile::WriteOnly);
    dest.write(src.readAll());
}

void CommandManagement::prepareCommandsSystem() {
    QSettings ini(iniFilePath, QSettings::IniFormat);
    QString sectionName;
    QString cmd;
    int slashIndex;
    QString shortcut;

    foreach(QString key, ini.allKeys()) {
        if(key == "CommandPalette/1-commandPalette")
            commandPaletteShortcut = ini.value(key).toString();

        else {
            slashIndex           = key.indexOf('/');
            sectionName          = key.mid(0, slashIndex);
            // remove index from key value
            cmd                  = key.mid(key.indexOf('-') + 1, key.length());
            shortcut             = ini.value(key).toString();

            commandSlotDB->initSlotData(cmd, sectionName, shortcut);
            commandPaletteCommandManagement->addCommand(cmd, shortcut);
        }
    }
}

void CommandManagement::enableCommand(QString cmd) {
    QString shortcut = commandSlotDB->getSlotShortcut(cmd);
    QObject* slotObj = commandSlotDB->getSlotObject(cmd);
    CommandSlotDB::slot_func_t slot_func = commandSlotDB->getSlotFunc(cmd);

    // activate command at command pallete
    commandPaletteCommandManagement->activateCommand(cmd);

    // shortcut
    connect( new QShortcut(QKeySequence(shortcut), qobject_cast<QWidget*>(slotObj)),
             &QShortcut::activated,
             slotObj,
             _SLOT(slot_func()) );
}

void CommandManagement::disableCommand(QString cmd) {
    commandPaletteCommandManagement->deactivateCommand(cmd);
}

void CommandManagement::onCommandActivation(QString cmd) {
    commandSlotDB->getSlotFunc(cmd)();
}

CommandManagement::~CommandManagement() {
    delete commandPaletteCommandManagement;
}

/*============================================================================*/

CommandPaletteCommandManagement::
CommandPaletteCommandManagement( CommandPalette *commandPalette,
                                 QWidget* parent) : QObject(parent) {
    this->commandPalette = commandPalette;

    // hide commandpalette on `Esc` key pressed or on losing focus
    connect( commandPalette,
             &CommandPalette::focusLost,
             this,
             &CommandPaletteCommandManagement::hideCommandPalette );
    connect( new QShortcut(QKeySequence(Qt::Key_Escape), parent),
             &QShortcut::activated,
             this,
             &CommandPaletteCommandManagement::hideCommandPalette );

    // commandpalette command activation signal and slot
    connect( commandPalette,
             &CommandPalette::commandactivated,
             this,
             &CommandPaletteCommandManagement::onCommandActivation
           );
}

QString CommandPaletteCommandManagement::prepareCommand( QString cmd,
                                                         bool toCommandPalette) {
    if(toCommandPalette) {
        cmd.replace('_', ' ');
        cmd[0] = cmd[0].toUpper();
    }

    else {
        cmd.replace(' ', '_');
        cmd[0] = cmd[0].toLower();
    }

    return cmd;
}

void CommandPaletteCommandManagement::addCommand(QString cmd, QString shortcut) {
    cmd = this->prepareCommand(cmd, true);
    commandPalette->addCommand(cmd, shortcut, false);
}

bool CommandPaletteCommandManagement::activateCommand(QString cmd) {
    return commandPalette->enableCommand(prepareCommand(cmd, true));
}

bool CommandPaletteCommandManagement::deactivateCommand(QString cmd) {
    return commandPalette->enableCommand(prepareCommand(cmd, false));
}

void CommandPaletteCommandManagement::showCommandPalette() {
    // make commandpallette 2/3 width of its parent
    commandPalette->setMinimumWidth(commandPalette->parentWidget()->width() * 2/3);
    commandPalette->setVisible(true);
}

void CommandPaletteCommandManagement::hideCommandPalette() {
    commandPalette->setVisible(false);
}

void CommandPaletteCommandManagement::toggleCommandPalette() {
    if(commandPalette->isHidden())
        showCommandPalette();

    else
        hideCommandPalette();
}

void CommandPaletteCommandManagement::onCommandActivation(QString cmd) {
    this->hideCommandPalette();

    cmd = prepareCommand(cmd, false);
    emit(commandActivated(cmd));
}

CommandPaletteCommandManagement::~CommandPaletteCommandManagement() {
}

/*============================================================================*/
