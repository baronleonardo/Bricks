#ifndef BRICKS_H
#define BRICKS_H

#include <QMainWindow>
#include <QCloseEvent>
#include "commandmanagement.h"

namespace Ui {
class Bricks;
}

class Bricks : public QMainWindow
{
    Q_OBJECT

public:
    explicit Bricks(QWidget *parent = 0);
    ~Bricks();

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::Bricks *ui;
    CommandManagement* commandManagement = nullptr;
    const QString appTitle = tr("Bricks");
};

#endif // BRICKS_H
