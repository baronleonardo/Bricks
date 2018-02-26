#ifndef COMMANDPALETTE_H
#define COMMANDPALETTE_H

#include <QLineEdit>
#include <QWidget>

class CommandPalette : public QLineEdit
{
public:
    CommandPalette(QWidget* parent = nullptr);
    ~CommandPalette();
};

#endif // COMMANDPALETTE_H
