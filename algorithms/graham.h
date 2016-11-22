#ifndef GRAHAM_H
#define GRAHAM_H
#include <QPoint>
#include <QVector>
#include <stack>
namespace graham
{
int distSquare(QPoint p1, QPoint p2);
int orientation(QPoint p, QPoint q, QPoint r);
void convexHull(const QVector<QPoint> &pointsArray, QVector<int> &convexHull);
int compare(const QPoint & p1, const QPoint & p2);
}

#endif // GRAHAM_H
