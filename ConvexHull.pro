#-------------------------------------------------
#
# Project created by QtCreator 2016-11-22T20:09:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ConvexHull
TEMPLATE = app
greaterThan(QT_MAJOR_VERSION, 4){
CONFIG += c++11
} else {
QMAKE_CXXFLAGS += -std=c++0x
}

SOURCES += main.cpp\
        mainwindow.cpp \
    drawwidget.cpp \
    algorithms/jarvis.cpp \
    algorithms/graham.cpp

HEADERS  += mainwindow.h \
    drawwidget.h \
    algorithms/jarvis.h \
    algorithms/graham.h

FORMS    += mainwindow.ui
