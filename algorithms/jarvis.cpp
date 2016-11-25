#include "jarvis.h"

int Jarvis::getSideSign(QPoint a, QPoint b, QPoint c)
{
    return a.x() * (b.y() - c.y()) +
           b.x() * (c.y() - a.y()) +
           c.x() * (a.y() - b.y());
}

double Jarvis::dist(QPoint a, QPoint b)
{
    return sqrt( static_cast<double>(a.x() - b.x()) *
                 static_cast<double>(a.x() - b.x()) +
                 static_cast<double>(a.y() - b.y()) *
                 static_cast<double>(a.y() - b.y()));
}

bool Jarvis::isInside(QPoint a, QPoint b, QPoint c)
{
    return a.x() <= b.x() &&
           b.x() <= c.x() &&
           a.y() <= b.y() &&
            b.y() <= c.y();
}

double Jarvis::getDelayMultiplier()
{
    return 100.0/_speed;
}

void Jarvis::convexHull(const QVector<QPoint> & pointsArray, QVector<int> & convexHull)
{
    int size = pointsArray.size();
    int first = 0;
    for (int i=1; i<size; i++) {
        if (pointsArray[i].y() < pointsArray[first].y()){
            first = i;
        }
        else if (pointsArray[i].y() == pointsArray[first].y() &&
                 pointsArray[i].x() < pointsArray[first].x()){
            first = i;
        }
        emit setPoint(pointsArray[first]);
        this->msleep(15 * getDelayMultiplier());
    }
    convexHull.push_back(first);


    int cur = first;
    do {
        int next = (cur + 1) % size;
        emit setPoint(pointsArray[cur]);
        this->msleep(5 * getDelayMultiplier());
        for (int i = 0; i < size; i++) {
            int sign = getSideSign(pointsArray[cur], pointsArray[next], pointsArray[i]);
            if (sign < 0) {
                next = i;
            }
            else if (sign == 0) {
                if (isInside(pointsArray[cur],pointsArray[next],pointsArray[i])) {
                    next = i;
                }
            }
            emit setLineRed(pointsArray[cur],pointsArray[i]);
            this->msleep(20 * getDelayMultiplier());
        }
        cur = next;
        if(cur != first) {
            convexHull.push_back(next);
            emit setHull(convexHull);
        }
    } while (cur != first);


}

void Jarvis::setInputData(int speed, const QVector<QPoint> &pointsArray)
{
    _speed = speed;
    _pointsArray = pointsArray;
}

void Jarvis::setSpeed(int speed)
{
    _speed = speed;
}

void Jarvis::run()
{
    convexHull(_pointsArray,_convexHull);
}

