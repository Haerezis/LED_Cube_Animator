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

HEADERS += include/ActionsSlot.hpp \
           include/utils.hpp \
           include/Vertex.hpp \
           include/SphereVertices.hpp \
           include/Animation.hpp \
           include/AnimationFrame.hpp \
           include/UtilsOpenGL.hpp \
           include/CubeOpenGL.hpp \
           include/LedOpenGL.hpp \
           include/LedMonocolorOpenGL.hpp \
           include/LedOnOpenGL.hpp \
           include/LedOffOpenGL.hpp \
           include/AnimationController.hpp \
#           include/AnimationFrameController.hpp \
           include/rapidjson/allocators.h \
           include/rapidjson/document.h \
           include/rapidjson/encodedstream.h \
           include/rapidjson/encodings.h \
           include/rapidjson/filereadstream.h \
           include/rapidjson/filewritestream.h \
           include/rapidjson/memorybuffer.h \
           include/rapidjson/memorystream.h \
           include/rapidjson/prettywriter.h \
           include/rapidjson/rapidjson.h \
           include/rapidjson/reader.h \
           include/rapidjson/stringbuffer.h \
           include/rapidjson/writer.h \
           include/rapidjson/error/en.h \
           include/rapidjson/error/error.h \
           include/rapidjson/internal/biginteger.h \
           include/rapidjson/internal/diyfp.h \
           include/rapidjson/internal/dtoa.h \
           include/rapidjson/internal/ieee754.h \
           include/rapidjson/internal/itoa.h \
           include/rapidjson/internal/meta.h \
           include/rapidjson/internal/pow10.h \
           include/rapidjson/internal/stack.h \
           include/rapidjson/internal/strfunc.h \
           include/rapidjson/internal/strtod.h \
           include/rapidjson/msinttypes/inttypes.h \
           include/rapidjson/msinttypes/stdint.h
FORMS += UI/GenerationOptionsDialog.ui \
         UI/MainWindows.ui \
         UI/NewAnimationDialog.ui
SOURCES += src/ActionsSlot.cpp \
           src/utils.cpp \
           src/Vertex.cpp \
           src/SphereVertices.cpp \
           src/Animation.cpp \
           src/AnimationFrame.cpp \
           src/UtilsOpenGL.cpp \
           src/CubeOpenGL.cpp \
           src/LedOpenGL.cpp \
           src/LedMonocolorOpenGL.cpp \
           src/LedOnOpenGL.cpp \
           src/LedOffOpenGL.cpp \
           src/AnimationController.cpp \
#           src/AnimationFrameController.cpp \
           src/main.cpp
RESOURCES += qdarkstyle/style.qrc

