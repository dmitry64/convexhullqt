#include "graham.h"

namespace graham
{

int distSquare(QPoint p1, QPoint p2)
{
    return (p1.x() - p2.x())*(p1.x() - p2.x()) +
           (p1.y() - p2.y())*(p1.y() - p2.y());
}

int orientation(QPoint p, QPoint q, QPoint r)
{
    int val = (q.y() - p.y()) * (r.x() - q.x()) -
              (q.x() - p.x()) * (r.y() - q.y());

    if (val == 0) return 0;
    return (val > 0)? 1: 2;
}

int compare(const QPoint &p1, const QPoint &p2, const QPoint &p0)
{
    int o = orientation(p0, p1, p2);
    if (o == 0)
        return (distSquare(p0, p2) >= distSquare(p0, p1))? -1 : 1;
    return (o == 2)? -1: 1;
}

void sort(const QVector<QPoint> &pointsArray, QVector<int> & pointsIndexes, QPoint & p0)
{
    int i = 0, j = 0, flag = 1;
    int temp;
    int len = pointsIndexes.size();
    for(i = 1; (i <= len) && flag; i++) {
        flag = 0;
        for (j=0; j < (len -1); j++) {
            if (compare(pointsArray[pointsIndexes[j+1]],pointsArray[pointsIndexes[j]],p0) < 0) {
                temp = pointsIndexes[j];
                pointsIndexes[j] = pointsIndexes[j+1];
                pointsIndexes[j+1] = temp;
                flag = 1;
            }
        }
    }
    return;
}

void convexHull(const QVector<QPoint> &pointsArray, QVector<int> &convexHull)
{
    QVector<int> pointsIndexes;
    for(int i=0; i<pointsArray.size(); i++) {
        pointsIndexes.push_back(i);
    }
    int n = pointsArray.size();
    int ymin = pointsArray[0].y();
    int min = 0;
    for (int i = 1; i < n; i++) {
        int y = pointsArray[i].y();
        if ((y < ymin) || (ymin == y && pointsArray[i].x() < pointsArray[min].x())) {
            ymin = pointsArray[i].y();
            min = i;
        }
    }

    int temp = pointsIndexes[0];
    pointsIndexes[0] = pointsIndexes[min];
    pointsIndexes[min] = temp;

    QPoint p0 = pointsArray[pointsIndexes[0]];
    sort( pointsArray,pointsIndexes,p0);

    int newSize = 1;
    for (int i=1; i<n; i++) {
        while (i < n-1 && orientation(p0,
                                      pointsArray[pointsIndexes[i]],
                                      pointsArray[pointsIndexes[i+1]]) == 0) {
            i++;
        }
        pointsIndexes[newSize] = pointsIndexes[i];
        newSize++;
    }
    if (newSize < 3) return;

    convexHull.push_front(pointsIndexes[0]);
    convexHull.push_front(pointsIndexes[1]);
    convexHull.push_front(pointsIndexes[2]);

    for (int i = 3; i < newSize; i++) {
        while (orientation(pointsArray[convexHull[1]],
                           pointsArray[convexHull[0]],
                           pointsArray[pointsIndexes[i]]) != 2) {
            convexHull.pop_front();
        }
        convexHull.push_front(pointsIndexes[i]);
    }
    convexHull.push_front(pointsIndexes[0]);
}

}
