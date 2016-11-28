#include "divideandconquer.h"

void DivideAndConquer::run()
{
    convexHull(_pointsArray,_convexHull);
}

void DivideAndConquer::setInputData(int speed, const QVector<QPoint> &pointsArray)
{
    _speed = speed;
    _pointsArray = pointsArray;
}

void DivideAndConquer::setSpeed(int speed)
{
    _speed = speed;
}

void DivideAndConquer::sort(const QVector<QPoint> &pointsArray, QVector<int> & pointsIndexes)
{
    int i = 0, j = 0, flag = 1;
    int temp;
    int len = pointsIndexes.size();
    for(i = 1; (i <= len) && flag; i++) {
        flag = 0;
        for (j=0; j < (len -1); j++) {
            if (pointsArray[pointsIndexes[j+1]].x() < pointsArray[pointsIndexes[j]].x()) {
                temp = pointsIndexes[j];
                pointsIndexes[j] = pointsIndexes[j+1];
                pointsIndexes[j+1] = temp;
                flag = 1;
            }
        }
    }
    return;
}

double DivideAndConquer::getDelayMultiplier()
{
    return (100.0/_speed) * 0.5;
}

void DivideAndConquer::convexHull(const QVector<QPoint> &pointsArray, QVector<int> &convexHull)
{
    QVector<int> pointsIndexes;
    for(int i=0;i<pointsArray.size();i++) {
        pointsIndexes.append(i);
    }

    sort(pointsArray,pointsIndexes);

    QVector<int> _convexHull = getHullFromSlice(pointsArray,pointsIndexes,0,pointsIndexes.size()-1);
    emit setHull(_convexHull);
}


QVector<int> DivideAndConquer::getHullFromSlice(const QVector<QPoint> &pointsArray, QVector<int> &pointsIndexes, int from, int to)
{
    if(to-from == 2) {
        QVector<int> out;

        QVector<int> points;
        points.push_back(pointsIndexes[from]);
        points.push_back(pointsIndexes[from+1]);
        points.push_back(pointsIndexes[from+2]);

        int orient = orientation(pointsArray[points[0]],pointsArray[points[1]],pointsArray[points[2]]);

        if(orient==1) {
            out.push_back(points[0]);
            out.push_back(points[1]);
            out.push_back(points[2]);
        } else if (orient==2) {
            out.push_back(points[0]);
            out.push_back(points[2]);
            out.push_back(points[1]);
        }

        return out;
    } else if(to-from == 1) {
        QVector<int> out;
        if(pointsArray[pointsIndexes[to]].y() < pointsArray[pointsIndexes[from]].y()) {
            out.push_back(pointsIndexes[from]);
            out.push_back(pointsIndexes[to]);
        } else {
            out.push_back(pointsIndexes[to]);
            out.push_back(pointsIndexes[from]);
        }
        return out;
    } else if(to == from) {
        QVector<int> out;
        out.push_back(pointsIndexes[to]);
        return out;
    }
    else {
        int middle = from + (to-from)/2;

        QPoint leftPoint = pointsArray[pointsIndexes[middle]];
        QPoint rightPoint = pointsArray[pointsIndexes[middle+1]];
        int coordMiddle = leftPoint.x() + (rightPoint.x() - leftPoint.x()) / 2;
        QPoint topDiv = QPoint(coordMiddle,10000);
        QPoint bottomDiv = QPoint(coordMiddle,-10000);
        emit setLineRed(topDiv,bottomDiv);
        this->msleep(80 * getDelayMultiplier());

        QVector<int> left = getHullFromSlice(pointsArray,pointsIndexes,from,middle);
        emit setLineRed(topDiv,bottomDiv);
        this->msleep(70 * getDelayMultiplier());

        QVector<int> right = getHullFromSlice(pointsArray,pointsIndexes,middle+1,to);
        emit setLineRed(topDiv,bottomDiv);
        emit clearHull();
        emit setMerge(left,right);
        this->msleep(70 * getDelayMultiplier());

        QVector<int> result = merge(pointsArray,pointsIndexes,left,right);
        emit resetMarkers();
        emit setHull(result);
        this->msleep(200 * getDelayMultiplier());
        return result;
    }
}

QVector<int> DivideAndConquer::merge(const QVector<QPoint> &pointsArray, QVector<int> &pointsIndexes, QVector<int> left, QVector<int> right)
{
    QVector<int> result;

    int topLeft = 0;
    int topRight = 0;
    int bottomLeft = 0;
    int bottomRight = 0;

    bool topFound = false;
    bool bottomFound = false;

    for(int i = 0; i < left.size(); i++) {
        if(topFound&&bottomFound) {
            break;
        }
        for(int j = 0; j < right.size(); j++) {
            QPoint lPoint = pointsArray[left[i]];
            QPoint rPoint = pointsArray[right[j]];
            if(!(lPoint.x()==rPoint.x() && lPoint.y() == rPoint.y())) {
                emit setLineRed(lPoint,rPoint);
                this->msleep(50 * getDelayMultiplier());
                if(lPoint.x()==rPoint.x()) {
                    // TODO: write some code here
                } else {
                    double m = static_cast<double>(lPoint.y() - rPoint.y()) / static_cast<double>(lPoint.x() - rPoint.x());
                    double b = (m * (-lPoint.x())) + lPoint.y();
                    bool leftIsLower = true;
                    bool leftIsHigher = true;
                    bool rightIsLower = true;
                    bool rightIsHigher = true;
                    for(int k = 0; k < left.size(); k++){
                        QPoint curPoint = pointsArray[left[k]];
                        if(curPoint!=lPoint) {
                            if(m*curPoint.x() + b > curPoint.y()) {
                                leftIsHigher = false;
                            }
                            if(m*curPoint.x() + b < curPoint.y()) {
                                leftIsLower = false;
                            }
                        }
                    }
                    for(int k = 0; k < right.size(); k++){
                        QPoint curPoint = pointsArray[right[k]];
                        if(curPoint!=rPoint) {
                            if(m*curPoint.x() + b > curPoint.y()) {
                                rightIsHigher = false;
                            }
                            if(m*curPoint.x() + b < curPoint.y()) {
                                rightIsLower = false;
                            }
                        }
                    }

                    if(rightIsLower && leftIsLower) {
                        topFound = true;
                        topLeft = i;
                        topRight = j;
                        emit setLineYellow(lPoint,rPoint);
                        this->msleep(100 * getDelayMultiplier());
                    }
                    if(rightIsHigher && leftIsHigher) {
                        bottomFound = true;
                        bottomLeft = i;
                        bottomRight = j;
                        emit setLineYellow(lPoint,rPoint);
                        this->msleep(100 * getDelayMultiplier());
                    }
                    if(rightIsHigher && leftIsHigher && rightIsLower && leftIsLower) {
                        qDebug("Error!!!!!");
                    }

                    if(topFound&&bottomFound) {
                        break;
                    }
                }
            }
        }
    }

    if(bottomLeft <= topLeft) {
        for(int i = 0; i<left.size(); i++){
            if(i >=bottomLeft && i<=topLeft) {
                result.push_back(left[i]);
                emit setPoint(pointsArray[left[i]]);
                this->msleep(50 * getDelayMultiplier());
            }
        }
    } else {
        for(int i = bottomLeft; i<left.size(); i++){
            result.push_back(left[i]);
            emit setPoint(pointsArray[left[i]]);
            this->msleep(50 * getDelayMultiplier());
        }
        for(int i = 0; i<=topLeft; i++){
            result.push_back(left[i]);
            emit setPoint(pointsArray[left[i]]);
            this->msleep(50 * getDelayMultiplier());
        }
    }

    if(bottomRight >= topRight) {
        for(int i = 0; i<right.size(); i++){
            if(i>=topRight && i <=bottomRight) {
                result.push_back(right[i]);
                emit setPoint(pointsArray[right[i]]);
                this->msleep(50 * getDelayMultiplier());
            }
        }
    } else {
        for(int i = topRight; i<right.size(); i++){
            result.push_back(right[i]);
            emit setPoint(pointsArray[right[i]]);
            this->msleep(50 * getDelayMultiplier());
        }
        for(int i = 0; i<=bottomRight; i++){
            result.push_back(right[i]);
            emit setPoint(pointsArray[right[i]]);
            this->msleep(50 * getDelayMultiplier());
        }
    }

    return result;
}
