#ifndef COMMANDS_H
#define COMMANDS_H

#include <QObject>

class Commands : public QObject
{
    Q_OBJECT

public:
    explicit Commands(QObject *parent = nullptr);
    ~Commands();

signals:

public slots:
};

#endif // COMMANDS_H
