#include "bricks.h"
#include "ui_bricks.h"
#include "tabmanagement.h"

Bricks::Bricks(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Bricks)
{
    ui->setupUi(this);
    ui->tabWidget->newTab();
}

Bricks::~Bricks()
{
    delete ui;
}
