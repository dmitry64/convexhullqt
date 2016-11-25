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
    connect(this,SIGNAL(runDivideAndConquer()),ui->drawWidget,SLOT(runDivideAndConquer()));
    connect(this,SIGNAL(clearAll()),ui->drawWidget,SLOT(clearAll()));
    connect(this,SIGNAL(stop()),ui->drawWidget,SLOT(stopAllAlgorithms()));
    connect(this,SIGNAL(pointsCountChanged(int)),ui->drawWidget,SLOT(onPointsCountChanged(int)));
    connect(this,SIGNAL(speedChanged(int)),ui->drawWidget,SLOT(onSpeedChanged(int)));
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

void MainWindow::on_dacButton_released()
{
    emit runDivideAndConquer();
}

void MainWindow::on_clearButton_released()
{
    emit clearAll();
}

void MainWindow::on_stopButton_released()
{
    emit stop();
}

void MainWindow::on_pointsSlider_valueChanged(int value)
{
    emit pointsCountChanged(value);
}

void MainWindow::on_speedSlider_valueChanged(int value)
{
    emit speedChanged(value);
}
