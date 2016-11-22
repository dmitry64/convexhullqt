#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this,SIGNAL(generatePoints()),ui->drawWidget,SLOT(generatePoints()));
    connect(this,SIGNAL(runJarvis()),ui->drawWidget,SLOT(runJarvis()));
    connect(this,SIGNAL(runGraham()),ui->drawWidget,SLOT(runGraham()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_generateButton_released()
{
    emit generatePoints();
}

void MainWindow::on_jarvisButton_released()
{
    emit runJarvis();
}

void MainWindow::on_grahamButton_released()
{
    emit runGraham();
}

void MainWindow::on_mergeButton_released()
{

}
