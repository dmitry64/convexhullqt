#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H
#include <QVector>
#include <QPoint>
#include <QWidget>
#include <QPen>
#include <algorithms/jarvis.h>
#include <algorithms/graham.h>
#include <algorithms/divideandconquer.h>

class DrawWidget : public QWidget
{
    Q_OBJECT

    QVector<QPoint> _points;
    QVector<int> _convexHull;
    double _scaleX;
    double _scaleY;
    QPen _linePen;
    QPen _textPen;
    QPen _pointPen;
    QPen _hullPointPen;
    QPen _axisPen;
    QPen _specialPointPen;
    QPen _redLinePen;
    QPen _yellowLinePen;
    QPen _mergePen;
    QBrush _brush;

    QLine _redLine;
    QLine _yellowLine;

    QPoint _specialPoint;

    QVector<int> _mergeHull1;
    QVector<int> _mergeHull2;

    Jarvis * _jarvis;
    Graham * _graham;
    DivideAndConquer * _divideAndConquer;

    int _speed;
    int _pointsCount;
public:
    explicit DrawWidget(QWidget *parent = 0);

    QPoint toScreenCoords(QPoint & point);
    QLine toScreenCoords(QLine & line);
signals:
    void speedChanged(int speed);

public slots:
    void runJarvis();
    void runGraham();
    void runDivideAndConquer();
    void generatePoints();
    void resetMarkers();
    void clearHull();
    void clearAll();
    void paintEvent(QPaintEvent * event);
    void drawPoint(QPainter & painter, QPoint point);
    void drawVector(QPainter & painter, QVector<QPoint> & _points);
    void drawMerge(QPainter & painter);
    void setLineRed(QPoint a, QPoint b);
    void setLineYellow(QPoint a, QPoint b);
    void setPoint(QPoint a);
    void setHull(IntVector convexHull);
    void setMerge(IntVector hull1,IntVector hull2);
    void onAlgorithmFinished();
    void drawHull(QPainter & painter);
    void resizeEvent(QResizeEvent * event);
    void stopAllAlgorithms();
    void onPointsCountChanged(int value);
    void onSpeedChanged(int speed);

};

#endif // DRAWWIDGET_H
