#-------------------------------------------------
#
# Project created by QtCreator 2020-02-17T15:32:51
#
#-------------------------------------------------

QT       -= gui

TARGET = Utils
TEMPLATE = lib

DEFINES += UTILS_LIBRARY

DESTDIR += $$PWD/../bin
MOC_DIR += $$PWD/../Utils/temp
OBJECTS_DIR += $$PWD/../Utils/temp
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS_RELEASE += /Zi
QMAKE_CXXFLAGS_RELEASE += /Od
QMAKE_LFLAGS_RELEASE += /DEBUG

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    RALLBlockSignal.cpp \
    Utils.cpp

HEADERS += \
        RALLBlockSignal.h \
        Utils.h \
        utils_global.h 

unix {
    target.path = /usr/lib
    INSTALLS += target
}
