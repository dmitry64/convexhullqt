#ifndef GRAHAM_H
#define GRAHAM_H
#include <QPoint>
#include <QVector>
#include <QThread>
#include <common.h>

class Graham : public QThread {
    Q_OBJECT

    QVector<QPoint> _pointsArray;
    QVector<int> _convexHull;
    int _speed;
    int distSquare(QPoint p1, QPoint p2);
    int compare(const QPoint &p1, const QPoint &p2, const QPoint &p0);
    void sort(const QVector<QPoint> &pointsArray, QVector<int> & pointsIndexes, QPoint & p0);
    QVector<int> reverseHull(QVector<int> hull);
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

#endif // GRAHAM_H
