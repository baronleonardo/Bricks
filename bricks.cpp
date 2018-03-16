#include "bricks.h"
#include "ui_bricks.h"
#include "tabmanagement.h"
#include "commandpalette.h"
#include "commandslotdb.h"
#include <QApplication>
#include <QFileInfo>
#include <QDebug>

Bricks::Bricks(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Bricks)
{
    ui->setupUi(this);

    commandManagement = new CommandManagement(ui->commandPalette, this);
//    ui->commandPatternLayout->insertWidget(1, commandManagement->getCommandPalette());

    // change window name based on current tab name
    connect( ui->tabWidget,
             &TabManagement::tabNameChanged,
             [=](QString tabName){ this->setWindowTitle(tabName + " - " + this->appTitle); } );

    // register quit action
    CommandSlotDB* commandSlotDB = CommandSlotDB::getInstance();
    commandSlotDB->registerSlot("quit", this, _SLOT(QApplication::quit()));

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

        // if filepath is wrong, exit
        if(ui->tabWidget->count() == 0)
            exit(-1);
    }
}

void Bricks::closeEvent(QCloseEvent* event) {
    event->ignore();

    if(ui->tabWidget->closeAllTabs())
        event->accept();
}

Bricks::~Bricks()
{
    delete ui;
    delete commandManagement;
}
