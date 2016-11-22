#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H
#include <QVector>
#include <QPoint>
#include <QWidget>
#include <QPen>

class DrawWidget : public QWidget
{
    Q_OBJECT

    QVector<QPoint> _points;
    QVector<int> _convexHull;
    double _scale;
    QPen _linePen;
    QPen _textPen;
    QPen _pointPen;
    QPen _hullPointPen;
    QBrush _brush;

public:
    explicit DrawWidget(QWidget *parent = 0);
    void drawPoint(QPainter & painter, QPoint point);
    void drawVector(QPainter & painter, QVector<QPoint> & _points);
    void drawHull(QPainter & painter);

    QPoint toScreenCoords(QPoint & point);
signals:

public slots:
    void runJarvis();
    void runGraham();
    void generatePoints();
    void paintEvent(QPaintEvent * event);
};

#endif // DRAWWIDGET_H
