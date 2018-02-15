#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tabmanagement.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->newTab();
}

MainWindow::~MainWindow()
{
    delete ui;
}
