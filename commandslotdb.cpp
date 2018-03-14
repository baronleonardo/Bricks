#include "commandslotdb.h"
#include <QDebug>

CommandSlotDB* CommandSlotDB::getInstance(){
    static CommandSlotDB singletonInstance;
    return &singletonInstance;
}

void CommandSlotDB::initSlotData( QString slotName,
                                  QString sectionName,
                                  QString shortcut ) {
    _slots[slotName] = {sectionName, shortcut, NULL, NULL};
}

void CommandSlotDB::registerSlot( QString slotName,
                                  QObject* obj,
                                  slot_func_t slot_func ) {
    if(!_slots.contains(slotName))
        return;

    _slots[slotName].obj = obj;
    _slots[slotName].slot_func = slot_func;

    emit(slotRegistered(slotName));
}

QString CommandSlotDB::getSlotSectionName(QString slotName) {
    if(!_slots.contains(slotName))
        return "";
    else
        return _slots[slotName].sectionName;
}

QString CommandSlotDB::getSlotShortcut(QString slotName) {
    if(!_slots.contains(slotName))
        return "";
    else
        return _slots[slotName].shortcut;
}

QObject* CommandSlotDB::getSlotObject(QString slotName) {
    if(!_slots.contains(slotName))
        return nullptr;
    else
        return _slots[slotName].obj;
}

CommandSlotDB::slot_func_t CommandSlotDB::getSlotFunc(QString slotName) {
    if(!_slots.contains(slotName))
        return nullptr;
    else
        return _slots[slotName].slot_func;
}

