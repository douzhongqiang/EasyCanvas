#-------------------------------------------------
#
# Project created by QtCreator 2020-07-13T22:27:57
#
#-------------------------------------------------

QT       += gui widgets

TARGET = PythonWrap
TEMPLATE = lib

DEFINES += PYTHONWRAP_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR += $$PWD/../bin
MOC_DIR += $$PWD/../PythonWrap/temp
OBJECTS_DIR += $$PWD/../PythonWrap/temp

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -L$$PWD/../bin -lNDNodeAttribute
LIBS += -L$$PWD/../bin -lEasyCanvasCore

INCLUDEPATH += $$PWD/../NDNodeAttribute
INCLUDEPATH += $$PWD/../EasyCanvasCore

INCLUDEPATH += $$PWD/../thirdLibs/PythonQt/include
INCLUDEPATH += $$PWD/../thirdLibs/python3_8/include
CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../thirdLibs/PythonQt/libs/debug -lPythonQt_QtAll-Qt5-Python38_d -lPythonQt-Qt5-Python38_d
    LIBS += -L$$PWD/../thirdLibs/python3_8/libs -lpython3_d -lpython38_d
} else {
    LIBS += -L$$PWD/../thirdLibs/PythonQt/libs/release -lPythonQt_QtAll-Qt5-Python38 -lPythonQt-Qt5-Python38
    LIBS += -L$$PWD/../thirdLibs/python3_8/libs -lpython3 -lpython38
}

SOURCES += \
        PythonWrapCore.cpp \
        pythonwrap.cpp

HEADERS += \
        PythonWrapCore.h \
        pythonwrap.h \
        pythonwrap_global.h 

unix {
    target.path = /usr/lib
    INSTALLS += target
}
