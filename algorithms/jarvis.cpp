#include "jarvis.h"
namespace jarvis
{

int getSideSign(QPoint a, QPoint b, QPoint c)
{
    return a.x() * (b.y() - c.y()) +
           b.x() * (c.y() - a.y()) +
           c.x() * (a.y() - b.y());
}

double dist(QPoint a, QPoint b)
{
    return sqrt( static_cast<double>(a.x() - b.x()) *
                 static_cast<double>(a.x() - b.x()) +
                 static_cast<double>(a.y() - b.y()) *
                 static_cast<double>(a.y() - b.y()));
}

bool isInside(QPoint a, QPoint b, QPoint c)
{
    return a.x() <= b.x() &&
           b.x() <= c.x() &&
           a.y() <= b.y() &&
           b.y() <= c.y();
}

void convexHull(const QVector<QPoint> & pointsArray, QVector<int> & convexHull)
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
    }
    convexHull.push_back(first);

    int cur = first;
    do {
        int next = (cur + 1) % size;
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
        }
        cur = next;
        convexHull.push_back(next);
    } while (cur != first);
}

}
