#ifndef JARVIS_H
#define JARVIS_H
#include <QPoint>
#include <QVector>
#include <QThread>
#include <common.h>

class Jarvis : public QThread {
    Q_OBJECT

    QVector<QPoint> _pointsArray;
    QVector<int> _convexHull;

    int getSideSign(QPoint a, QPoint b, QPoint c);
    double dist(QPoint a, QPoint b);
    bool isInside(QPoint a, QPoint b, QPoint c);
    int _speed;
    double getDelayMultiplier();
signals:
    void setLineRed(QPoint a, QPoint b);
    void setPoint(QPoint a);
    void setHull(IntVector convexHull);

public:
    void run();
    void setInputData(int speed, const QVector<QPoint> &pointsArray);

    void convexHull(const QVector<QPoint> &pointsArray, QVector<int> &convexHull);
public slots:
    void setSpeed(int speed);
};

#endif // JARVIS_H
