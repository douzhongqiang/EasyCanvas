#-------------------------------------------------
#
# Project created by QtCreator 2020-01-27T13:54:45
#
#-------------------------------------------------

QT       += widgets

TARGET = NodeAttributeControl
TEMPLATE = lib

DEFINES += NODEATTRIBUTECONTROL_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR += $$PWD/../bin
MOC_DIR += $$PWD/../NodeAttributeControl/temp
OBJECTS_DIR += $$PWD/../NodeAttributeControl/temp
CONFIG += c++11

LIBS += -L$$PWD/../bin -lCustomWidgets
LIBS += -L$$PWD/../bin -lCustomControls
LIBS += -L$$PWD/../bin -lNDNodeAttribute

INCLUDEPATH += $$PWD/../CustomWidgets
INCLUDEPATH += $$PWD/../CustomControls
INCLUDEPATH += $$PWD/../NDNodeAttribute

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        UIAttrBoolControl.cpp \
        UIAttrColorControl.cpp \
        UIAttrFloatControl.cpp \
        UIAttrIntControl.cpp \
        UIAttrTextControl.cpp \
        UINodeAttrControl.cpp

HEADERS += \
        UIAttrBoolControl.h \
        UIAttrColorControl.h \
        UIAttrFloatControl.h \
        UIAttrIntControl.h \
        UIAttrTextControl.h \
        UINodeAttrControl.h \
        nodeattributecontrol_global.h 

unix {
    target.path = /usr/lib
    INSTALLS += target
}
