#ifndef COMMANDSLOTDB_H
#define COMMANDSLOTDB_H

#include <QObject>
#include <QString>
#include <QMap>
#include <functional>

#define _SLOT(func) ([=](){func;})

class CommandSlotDB: public QObject {
    Q_OBJECT

private:
    CommandSlotDB() {}
    ~CommandSlotDB() {}
    CommandSlotDB(CommandSlotDB const&) = delete;
    void operator=(CommandSlotDB const&) = delete;

public:
    typedef std::function<void(void)> slot_func_t;

signals:
    void slotRegistered(QString);

private:
    typedef struct {
        QString sectionName;
        QString shortcut;
        QObject* obj;
        slot_func_t slot_func;
    } Slot;

private:
    QMap<QString, CommandSlotDB::Slot> _slots;

public:
    static CommandSlotDB* getInstance();
    void initSlotData(QString slotName, QString sectionName, QString shortcut);
    void registerSlot(QString slotName, QObject* obj, slot_func_t slot_func);
    QString getSlotSectionName(QString slotName);
    QString getSlotShortcut(QString slotName);
    QObject* getSlotObject(QString slotName);
    slot_func_t getSlotFunc(QString slotName);
};

#endif // COMMANDSLOTDB_H
