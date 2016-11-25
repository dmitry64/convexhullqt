#include "drawwidget.h"
#include <QPainter>
#include <QPoint>
#include <random>
#include <QDebug>



DrawWidget::DrawWidget(QWidget *parent) : QWidget(parent)
{
    qRegisterMetaType<IntVector>("IntVector");
    _scaleX = 0.06;
    _scaleY = 0.06;
    _linePen = QPen(QColor(10, 255, 10), 2, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);
    _textPen = QPen(QColor(255, 10, 10), 1);
    _pointPen = QPen(QColor(10, 10, 255), 3);
    _hullPointPen = QPen(QColor(255, 10, 10), 4);
    _axisPen = QPen(QColor(60,60,60), 1);
    _specialPointPen = QPen(QColor(255,10,10),6);
    _redLinePen = QPen(QColor(200,10,10),1);
    _yellowLinePen = QPen(QColor(230,230,10),1);
    _mergePen = QPen(QColor(10,150,150),1);
    _jarvis = 0;
    _graham = 0;
    _divideAndConquer = 0;
    _speed = 50;
    _pointsCount = 10;

    resetMarkers();
}

QPoint DrawWidget::toScreenCoords(QPoint &point)
{
    QPoint p = QPoint(point.x(),-point.y());
    p.setX(p.x() * _scaleX);
    p.setY(p.y() * _scaleY);
    QPoint center(width()/2,height()/2);
    return p + center;
}

QLine DrawWidget::toScreenCoords(QLine &line)
{

    QPoint p1 = line.p1();
    p1 = toScreenCoords(p1);
    QPoint p2 = line.p2();
    p2 = toScreenCoords(p2);
    return QLine(p1,p2);
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

void DrawWidget::drawMerge(QPainter &painter)
{
    for(int i=0; i<_mergeHull1.size()-1; i++) {
        int indexFirst = _mergeHull1.at(i);
        int indexSecond = _mergeHull1.at(i+1);
        QPoint first = toScreenCoords(_points[indexFirst]);
        QPoint second = toScreenCoords(_points[indexSecond]);
        painter.setPen(_mergePen);
        painter.drawLine(first,second);
        painter.drawText(first.x()-8,first.y()-8,QString::number(i));
    }
    if(_mergeHull1.size() > 2){
        painter.setPen(_mergePen);
        painter.drawLine(toScreenCoords(_points[_mergeHull1.last()]),toScreenCoords(_points[_mergeHull1.first()]));
        painter.drawText(toScreenCoords(_points[_mergeHull1.last()]).x()-8,
                         toScreenCoords(_points[_mergeHull1.last()]).y()-8,
                         QString::number(_mergeHull1.size()-1));
    }
    for(int i=0; i<_mergeHull2.size()-1; i++) {
        int indexFirst = _mergeHull2.at(i);
        int indexSecond = _mergeHull2.at(i+1);
        QPoint first = toScreenCoords(_points[indexFirst]);
        QPoint second = toScreenCoords(_points[indexSecond]);
        painter.setPen(_mergePen);
        painter.drawLine(first,second);
        painter.drawText(first.x()-8,first.y()-8,QString::number(i));
    }
    if(_mergeHull2.size() > 2) {
        painter.setPen(_mergePen);
        painter.drawLine(toScreenCoords(_points[_mergeHull2.last()]),toScreenCoords(_points[_mergeHull2.first()]));
        painter.drawText(toScreenCoords(_points[_mergeHull2.last()]).x()-8,
                         toScreenCoords(_points[_mergeHull2.last()]).y()-8,
                         QString::number(_mergeHull2.size()-1));

    }
}

void DrawWidget::setLineRed(QPoint a, QPoint b)
{
    _redLine = QLine(a,b);
    this->repaint();
}

void DrawWidget::setLineYellow(QPoint a, QPoint b)
{
    _yellowLine = QLine(a,b);
    this->repaint();
}

void DrawWidget::setPoint(QPoint a)
{
    _specialPoint = a;
    this->repaint();
}

void DrawWidget::setHull(IntVector convexHull)
{
    _convexHull = convexHull;
    this->repaint();
}

void DrawWidget::setMerge(IntVector hull1, IntVector hull2)
{
    _mergeHull1 = hull1;
    _mergeHull2 = hull2;
    this->repaint();
}

void DrawWidget::onAlgorithmFinished()
{
    resetMarkers();
}

void DrawWidget::drawHull(QPainter &painter)
{
    if(_convexHull.size() > 0) {
        if(_convexHull.size() == 1) {
            int indexFirst = _convexHull.at(0);
            QPoint first = toScreenCoords(_points[indexFirst]);
            painter.setPen(_hullPointPen);
            painter.drawPoint(first);
            painter.setPen(_textPen);
            painter.drawText(first.x()+8,first.y()+8,QString::number(0));
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
            painter.drawText(first.x()+8,first.y()+8,QString::number(i));
        }

        painter.setPen(_linePen);
        painter.drawLine(toScreenCoords(_points[_convexHull.last()]),toScreenCoords(_points[_convexHull.first()]));
        painter.setPen(_textPen);
        painter.drawText(toScreenCoords(_points[_convexHull.last()]).x()+8,
                         toScreenCoords(_points[_convexHull.last()]).y()+8,QString::number(_convexHull.size()-1));
    }
}

void DrawWidget::resizeEvent(QResizeEvent *event)
{
    resetMarkers();
    _scaleX = width()/8000.0;
    _scaleY = height()/6000.0;
    QWidget::resizeEvent(event);
}

void DrawWidget::stopAllAlgorithms()
{
    clearHull();
    resetMarkers();
    if(_jarvis!=0) {
        _jarvis->terminate();
        _jarvis->wait();
        delete _jarvis;
        _jarvis = 0;
    }
    if(_graham!=0) {
        _graham->terminate();
        _graham->wait();
        delete _graham;
        _graham = 0;
    }
    if(_divideAndConquer!=0) {
        _divideAndConquer->terminate();
        _divideAndConquer->wait();
        delete _divideAndConquer;
        _divideAndConquer = 0;
    }
}

void DrawWidget::onPointsCountChanged(int value)
{
    _pointsCount = value;
}

void DrawWidget::onSpeedChanged(int speed)
{
    _speed = speed;
    emit speedChanged(_speed);
}

void DrawWidget::runJarvis()
{
    qDebug() << "Running Jarvis algorithm";
    _convexHull.clear();
    stopAllAlgorithms();
    if(_points.size()>0) {
        _jarvis = new Jarvis();
        _jarvis->setInputData(_speed,_points);
        connect(_jarvis,SIGNAL(setLineRed(QPoint,QPoint)),this,SLOT(setLineRed(QPoint,QPoint)));
        connect(_jarvis,SIGNAL(setPoint(QPoint)),this,SLOT(setPoint(QPoint)));
        connect(_jarvis,SIGNAL(setHull(IntVector)),this,SLOT(setHull(IntVector)));
        connect(_jarvis,SIGNAL(finished()),this,SLOT(onAlgorithmFinished()));
        connect(this, SIGNAL(speedChanged(int)),_jarvis, SLOT(setSpeed(int)) );
        _jarvis->start();
    }
    this->repaint();
}

void DrawWidget::runGraham()
{
    qDebug() << "Running Graham algorithm";
    _convexHull.clear();
    stopAllAlgorithms();
    if(_points.size()>0) {
        _graham = new Graham();
        _graham->setInputData(_speed,_points);
        connect(_graham,SIGNAL(setLineRed(QPoint,QPoint)),this,SLOT(setLineRed(QPoint,QPoint)));
        connect(_graham,SIGNAL(setPoint(QPoint)),this,SLOT(setPoint(QPoint)));
        connect(_graham,SIGNAL(setHull(IntVector)),this,SLOT(setHull(IntVector)));
        connect(_graham,SIGNAL(finished()),this,SLOT(onAlgorithmFinished()));
        connect(this, SIGNAL(speedChanged(int)),_graham, SLOT(setSpeed(int)) );
        _graham->start();
    }
    this->repaint();
}

void DrawWidget::runDivideAndConquer()
{
    qDebug() << "Running Divide And Conquer algorithm";
    _convexHull.clear();
    stopAllAlgorithms();
    if(_points.size()>0) {
        _divideAndConquer = new DivideAndConquer();
        _divideAndConquer->setInputData(_speed,_points);
        connect(_divideAndConquer,SIGNAL(setLineRed(QPoint,QPoint)),this,SLOT(setLineRed(QPoint,QPoint)));
        connect(_divideAndConquer,SIGNAL(setLineYellow(QPoint,QPoint)),this,SLOT(setLineYellow(QPoint,QPoint)));
        connect(_divideAndConquer,SIGNAL(setPoint(QPoint)),this,SLOT(setPoint(QPoint)));
        connect(_divideAndConquer,SIGNAL(setHull(IntVector)),this,SLOT(setHull(IntVector)));
        connect(_divideAndConquer,SIGNAL(setMerge(IntVector,IntVector)),this,SLOT(setMerge(IntVector,IntVector)));
        connect(_divideAndConquer,SIGNAL(resetMarkers()),this,SLOT(resetMarkers()));
        connect(_divideAndConquer,SIGNAL(clearHull()),this,SLOT(clearHull()));
        connect(_divideAndConquer,SIGNAL(finished()),this,SLOT(onAlgorithmFinished()));
        connect(this, SIGNAL(speedChanged(int)),_divideAndConquer, SLOT(setSpeed(int)) );
        _divideAndConquer->start();
    }
    this->repaint();
}

void DrawWidget::generatePoints()
{
    clearAll();
    bool useNormalX = rand()%2;
    bool useNormalY = rand()%2;

    std::default_random_engine generator;
    generator.seed(time(NULL));
    std::normal_distribution<double> distribution(0,1.0);
    for(int i = 0; i < 3 * _pointsCount; i++) {
        double x;
        double y;
        if(useNormalX) {
            x = distribution(generator) * 1000;
        } else {
            x = rand() % 7000 - 3500;
        }
        if(useNormalY) {
            y = distribution(generator) * 1000;
        } else {
            y = rand() % 5000 - 2500;
        }
        _points.push_back(QPoint(x ,y));
    }
    resetMarkers();
    this->repaint();
}

void DrawWidget::resetMarkers()
{
    _redLine = QLine(QPoint(-100000,-100000),QPoint(-100000,-100000));
    _yellowLine = QLine(QPoint(-100000,-100000),QPoint(-100000,-100000));
    _specialPoint = QPoint(-100000,-100000);
    _mergeHull1.clear();
    _mergeHull2.clear();
    this->repaint();
}

void DrawWidget::clearHull()
{
    _convexHull.clear();
    this->repaint();
}

void DrawWidget::clearAll()
{
    _points.clear();
    _convexHull.clear();
    resetMarkers();
    stopAllAlgorithms();
}

void DrawWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int w = width();
    int h = height();
    painter.fillRect(0,0,w,h,Qt::white);
    painter.setPen(_axisPen);
    painter.drawLine(w/2,h,w/2,0);
    painter.drawLine(w,h/2,0,h/2);

    painter.setFont(QFont("Arial", 8));

    drawVector(painter,_points);
    drawHull(painter);

    painter.setPen(_specialPointPen);
    painter.drawPoint(toScreenCoords(_specialPoint));

    painter.setPen(_redLinePen);
    painter.drawLine(toScreenCoords(_redLine));

    painter.setPen(_yellowLinePen);
    painter.drawLine(toScreenCoords(_yellowLine));

    drawMerge(painter);

}
