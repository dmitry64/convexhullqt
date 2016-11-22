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
private slots:
    void on_generateButton_released();
    void on_jarvisButton_released();
    void on_grahamButton_released();
    void on_mergeButton_released();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
