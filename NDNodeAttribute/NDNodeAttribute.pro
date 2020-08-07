#-------------------------------------------------
#
# Project created by QtCreator 2020-01-27T11:54:33
#
#-------------------------------------------------

QT       += widgets

TARGET = NDNodeAttribute
TEMPLATE = lib

DEFINES += NDNODEATTRIBUTE_LIBRARY

DESTDIR += $$PWD/../bin
MOC_DIR += $$PWD/../NDNodeAttribute/temp
OBJECTS_DIR += $$PWD/../NDNodeAttribute/temp
CONFIG += c++11

LIBS += -L$$PWD/../bin -lUtils
INCLUDEPATH += $$PWD/../Utils

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        NDAttributeBase.cpp \
        NDAttributeData.cpp \
        NDAttributeGroup.cpp \
        NDBoolAttribute.cpp \
        NDColorAttribute.cpp \
        NDIntAttribute.cpp \
        NDNodeBase.cpp \
        NDNodeManager.cpp \
        NDPostionAttribute.cpp \
        NDRealAttribute.cpp \
        NDStringAttribute.cpp \
        NDTextNode.cpp

HEADERS += \
        NDAttributeBase.h \
        NDAttributeData.h \
        NDAttributeGroup.h \
        NDBoolAttribute.h \
        NDColorAttribute.h \
        NDIntAttribute.h \
        NDNodeBase.h \
        NDNodeManager.h \
        NDPostionAttribute.h \
        NDRealAttribute.h \
        NDStringAttribute.h \
        NDTextNode.h \
        ndnodeattribute_global.h 

unix {
    target.path = /usr/lib
    INSTALLS += target
}
