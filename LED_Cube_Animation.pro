#-------------------------------------------------
#
# Project created by QtCreator 2016-01-03T22:37:23
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LED_Cube_Animation
TEMPLATE = app
INCLUDEPATH = include/

SOURCES += src/*.cpp \
    CubeOpenGL.cpp

HEADERS  += include/*.hpp \
    CubeOpenGL.hpp
