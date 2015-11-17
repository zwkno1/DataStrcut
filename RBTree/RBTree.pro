#-------------------------------------------------
#
# Project created by QtCreator 2015-09-07T18:20:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RBTree
CONFIG   += app

TEMPLATE = app


SOURCES += main.cpp \
    showwidget.cpp \
    controlwidget.cpp

HEADERS += \
    rbtree.h \
    showwidget.h \
    controlwidget.h

QMAKE_CXXFLAGS = -std=c++11
QMAKE_LIBS = -lz

FORMS += \
    controlwidget.ui
QMAKE_CXXFLAGS = -std=c++14 -fext-numeric-literals
