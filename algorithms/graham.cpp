#include "graham.h"

int Graham::distSquare(QPoint p1, QPoint p2)
{
    return (p1.x() - p2.x())*(p1.x() - p2.x()) +
           (p1.y() - p2.y())*(p1.y() - p2.y());
}

int Graham::compare(const QPoint &p1, const QPoint &p2, const QPoint &p0)
{
    int o = orientation(p0, p1, p2);
    if (o == 0)
        return (distSquare(p0, p2) >= distSquare(p0, p1))? -1 : 1;
    return (o == 2)? -1: 1;
}

void Graham::sort(const QVector<QPoint> &pointsArray, QVector<int> & pointsIndexes, QPoint & p0)
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

QVector<int> Graham::reverseHull(QVector<int> hull)
{
    QVector<int> result;
    for(int i=hull.size()-1; i>=0;i--){
        result.push_back(hull[i]);
    }
    return result;
}

double Graham::getDelayMultiplier()
{
    return 100.0/_speed;
}

void Graham::convexHull(const QVector<QPoint> &pointsArray, QVector<int> &convexHull)
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
        emit setPoint(pointsArray[i]);
        this->msleep(20 * getDelayMultiplier());
    }
    emit setPoint(pointsArray[min]);
    this->msleep(60 * getDelayMultiplier());

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
    emit setHull(reverseHull(convexHull));
    this->msleep(40 * getDelayMultiplier());
    convexHull.push_front(pointsIndexes[1]);
    emit setHull(reverseHull(convexHull));
    this->msleep(40 * getDelayMultiplier());
    convexHull.push_front(pointsIndexes[2]);
    emit setHull(reverseHull(convexHull));
    this->msleep(40 * getDelayMultiplier());

    for (int i = 3; i < newSize; i++) {
        while (orientation(pointsArray[convexHull[1]],
                           pointsArray[convexHull[0]],
                           pointsArray[pointsIndexes[i]]) != 2) {
            emit setHull(reverseHull(convexHull));
            this->msleep(160 * getDelayMultiplier());
            convexHull.pop_front();
            emit setHull(reverseHull(convexHull));
            this->msleep(80 * getDelayMultiplier());
        }
        convexHull.push_front(pointsIndexes[i]);
        emit setHull(reverseHull(convexHull));
        this->msleep(80 * getDelayMultiplier());
    }
}

void Graham::run()
{
    convexHull(_pointsArray,_convexHull);
}

void Graham::setInputData(int speed, const QVector<QPoint> &pointsArray)
{
    _speed = speed;
    _pointsArray = pointsArray;
}

void Graham::setSpeed(int speed)
{
    _speed = speed;
}
