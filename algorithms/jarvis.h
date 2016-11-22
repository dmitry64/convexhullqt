#ifndef JARVIS_H
#define JARVIS_H
#include <QPoint>
#include <QVector>
namespace jarvis {
    int getSideSign(QPoint a, QPoint b, QPoint c);
    double dist(QPoint a, QPoint b);
    bool isInside(QPoint a, QPoint b, QPoint c);
    void convexHull(const QVector<QPoint> &pointsArray, QVector<int> &convexHull);
}

#endif // JARVIS_H
