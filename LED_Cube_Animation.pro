#-------------------------------------------------
#
# Project created by QtCreator 2016-01-03T22:37:23
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LED_Cube_Animation
TEMPLATE = app

CONFIG += qt debug c++11

INCLUDEPATH = include/
UI_DIR = include/
DESTDIR=bin
OBJECTS_DIR=generated_files
MOC_DIR=generated_files

SOURCES += src/main.cpp \
    src/CubeOpenGL.cpp \
    src/Animation.cpp \
    src/AnimationFrame.cpp \
    src/ActionsSlot.cpp

HEADERS  += include/CubeOpenGL.hpp \
    include/Animation.hpp \
    include/AnimationFrame.hpp \
    include/ActionsSlot.hpp

FORMS += UI/MainWindows.ui \
    UI/GenerationOptionsDialog.ui \
    UI/NewAnimationDialog.ui

RESOURCES += qdarkstyle/style.qrc
