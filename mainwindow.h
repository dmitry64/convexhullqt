#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void generatePoints();
    void runJarvis();
    void runGraham();
    void runDivideAndConquer();
    void clearAll();
    void stop();
    void pointsCountChanged(int count);
    void speedChanged(int speed);
private slots:
    void on_generateButton_released();
    void on_jarvisButton_released();
    void on_grahamButton_released();
    void on_dacButton_released();
    void on_clearButton_released();
    void on_stopButton_released();


    void on_pointsSlider_valueChanged(int value);

    void on_speedSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
