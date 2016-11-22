#include "drawwidget.h"
#include <QPainter>
#include <QPoint>
#include <random>
#include <QDebug>
#include <algorithms/jarvis.h>
#include <algorithms/graham.h>

QPoint DrawWidget::toScreenCoords(QPoint &point)
{
    QPoint p = point * _scale;
    QPoint center(width()/2,height()/2);
    return p + center;
}

DrawWidget::DrawWidget(QWidget *parent) : QWidget(parent)
{
    _scale = 0.06;
    _linePen = QPen(QColor(10, 255, 10), 2, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);
    _textPen = QPen(QColor(255, 10, 10), 1);
    _pointPen = QPen(QColor(10, 10, 255), 1);
    _hullPointPen = QPen(QColor(255, 10, 10), 4);
}

void DrawWidget::drawPoint(QPainter & painter, QPoint point)
{
    painter.setPen(_pointPen);
    painter.drawPoint(toScreenCoords(point));
}

void DrawWidget::drawVector(QPainter & painter, QVector<QPoint> &points)
{
    for(int i = 0; i < points.size(); i++) {
        drawPoint(painter,points[i]);
    }
}

void DrawWidget::drawHull(QPainter &painter)
{
    if(_convexHull.size()) {
        if(_convexHull.size() == 1) {
            int indexFirst = _convexHull.at(0);
            QPoint first = toScreenCoords(_points[indexFirst]);
            painter.setPen(_hullPointPen);
            painter.drawPoint(first);
            painter.setPen(_textPen);
            painter.drawText(first.x()+8,first.y()+8,"#"+QString::number(1));
        }
        for(int i=0; i<_convexHull.size()-1; i++) {
            int indexFirst = _convexHull.at(i);
            int indexSecond = _convexHull.at(i+1);
            QPoint first = toScreenCoords(_points[indexFirst]);
            QPoint second = toScreenCoords(_points[indexSecond]);
            painter.setPen(_linePen);
            painter.drawLine(first,second);
            painter.setPen(_hullPointPen);
            painter.drawPoint(first);
            painter.setPen(_textPen);
            painter.drawText(first.x()+8,first.y()+8,"#"+QString::number(i+1));
        }
    }
}

void DrawWidget::runJarvis()
{
    qDebug() << "Running Jarvis algorithm";
    _convexHull.clear();
    jarvis::convexHull(_points,_convexHull);
    this->update();
}

void DrawWidget::runGraham()
{
    qDebug() << "Running Graham algorithm";
    _convexHull.clear();
    graham::convexHull(_points,_convexHull);
    this->update();
}

void DrawWidget::generatePoints()
{
    _points.clear();
    _convexHull.clear();
    std::default_random_engine generator;
    generator.seed(time(NULL));
    std::normal_distribution<double> distribution(0,1.0);
    for(int i = 0; i < 50; i++) {
        double x = distribution(generator) * 1024;
        double y = distribution(generator) * 1024;
        _points.push_back(QPoint(x ,y));
    }
    this->update();
}

void DrawWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(0,0,width(),height(),Qt::white);

    drawVector(painter,_points);
    drawHull(painter);
}
