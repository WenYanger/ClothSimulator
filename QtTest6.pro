#-------------------------------------------------
#
# Project created by QtCreator 2016-03-23T19:59:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += opengl
LIBS += -framework opengl
LIBS += -framework glut
CONFIG += c++11

TARGET = QtTest6
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    openglwidget.cpp \
    defaulttypes.cpp \
    spring.cpp \
    force.cpp \
    velocity.cpp \
    mass.cpp \
    grid.cpp

HEADERS  += mainwindow.h \
    openglwidget.h \
    defaulttypes.h \
    spring.h \
    force.h \
    velocity.h \
    mass.h \
    grid.h
