#ifndef BRICKS_H
#define BRICKS_H

#include <QMainWindow>

namespace Ui {
class Bricks;
}

class Bricks : public QMainWindow
{
    Q_OBJECT

public:
    explicit Bricks(QWidget *parent = 0);
    ~Bricks();

private:
    Ui::Bricks *ui;
    const QString appTitle = "Bricks";
};

#endif // BRICKS_H
