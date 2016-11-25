#ifndef COMMON_H
#define COMMON_H
#include <QPoint>

typedef QVector<int> IntVector;

inline int orientation(QPoint p, QPoint q, QPoint r)
{
    int val = (q.y() - p.y()) * (r.x() - q.x()) -
              (q.x() - p.x()) * (r.y() - q.y());

    if (val == 0) return 0;
    return (val > 0)? 1: 2;
}

#endif // COMMON_H
