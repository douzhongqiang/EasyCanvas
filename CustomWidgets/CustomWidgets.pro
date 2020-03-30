#-------------------------------------------------
#
# Project created by QtCreator 2020-01-04T20:25:00
#
#-------------------------------------------------

QT       += widgets

TARGET = CustomWidgets
TEMPLATE = lib

DEFINES += CUSTOMWIDGETS_LIBRARY

DESTDIR += $$PWD/../lib/CustomWidgets
MOC_DIR += $$PWD/../CustomWidgets/temp
OBJECTS_DIR += $$PWD/../CustomWidgets/temp
CONFIG += c++11

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
    UICustomCore/CustomStyleConfig.cpp \
    UICustomWidgtes/CustomDialog.cpp \
    UICustomWidgtes/CustomMainWindow.cpp \
    UICustomWidgtes/CustomTestWidget.cpp \
    UICustomWidgtes/CustomWidget.cpp

HEADERS += \
        UICustomCore/CustomStyleConfig.h \
        UICustomWidgtes/CustomDialog.h \
        UICustomWidgtes/CustomMainWindow.h \
        UICustomWidgtes/CustomTestWidget.h \
        UICustomWidgtes/CustomWidget.h \
        customwidgets_global.h 

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    qss.qrc
