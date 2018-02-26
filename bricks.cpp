#include "bricks.h"
#include "ui_bricks.h"
#include "tabmanagement.h"
#include <QApplication>
#include <QFileInfo>

Bricks::Bricks(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Bricks)
{
    ui->setupUi(this);

    // if no file path in args, just open blank tab at startup
    if(QApplication::arguments().length() == 1) {
        ui->tabWidget->newTab();
    }

    // if there is file path search for it
    // and open it at start up
    else {
        QFileInfo info;
        for(int iii = 1; iii < QApplication::arguments().length(); ++iii) {
            info.setFile(QApplication::arguments()[iii]);

            if(info.isFile()) {
                ui->tabWidget->openTab(info.filePath());
                break;
            }
        }
    }

    // change window name based on current tab name
    connect( ui->tabWidget,
             &TabManagement::tabNameChanged,
             [=](QString tabName){ this->setWindowTitle(tabName + " - " + this->appTitle); } );
}

Bricks::~Bricks()
{
    delete ui;
}
