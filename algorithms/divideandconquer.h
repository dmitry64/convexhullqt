#ifndef DIVIDEANDCONQUER_H
#define DIVIDEANDCONQUER_H

#include <QPoint>
#include <QVector>
#include <QThread>
#include <common.h>

class DivideAndConquer : public QThread {
    Q_OBJECT

    QVector<QPoint> _pointsArray;
    QVector<int> _convexHull;
    void sort(const QVector<QPoint> &pointsArray, QVector<int> & pointsIndexes);
    int _speed;
    double getDelayMultiplier();
signals:
    void setLineRed(QPoint a, QPoint b);
    void setLineYellow(QPoint a, QPoint b);
    void setPoint(QPoint a);
    void setHull(IntVector convexHull);
    void setMerge(IntVector hull1,IntVector hull2);
    void resetMarkers();
    void clearHull();

public:
    void run();
    void setInputData(int speed, const QVector<QPoint> &pointsArray);
    void convexHull(const QVector<QPoint> &pointsArray, QVector<int> &convexHull);
    QVector<int> getHullFromSlice(const QVector<QPoint> &pointsArray, QVector<int> &pointsIndexes, int from, int to);
    QVector<int> merge(const QVector<QPoint> &pointsArray, QVector<int> &pointsIndexes, QVector<int> left, QVector<int> right);
public slots:
    void setSpeed(int speed);
};

#endif // DIVIDEANDCONQUER_H
