#-------------------------------------------------
#
# Project created by QtCreator 2020-03-20T10:38:32
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EasyCanvas
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
RC_ICONS = EasyCanvas.ico

MOC_DIR += $$PWD/../temp/EasyCanvas
OBJECTS_DIR += $$PWD/../temp/EasyCanvas
DESTDIR += $$PWD/../bin

LIBS += -L$$PWD/../bin -lCustomWidgets
LIBS += -L$$PWD/../bin -lCustomControls
LIBS += -L$$PWD/../bin -lUtils
LIBS += -L$$PWD/../bin -lNDNodeAttribute
LIBS += -L$$PWD/../bin -lNodeAttributeControl
LIBS += -L$$PWD/../bin -lAudioVideoCore
LIBS += -L$$PWD/../bin -lPythonWrap
LIBS += -L$$PWD/../bin -lEasyCanvasCore

INCLUDEPATH += $$PWD/../CustomWidgets
INCLUDEPATH += $$PWD/../CustomControls
INCLUDEPATH += $$PWD/../NDNodeAttribute
INCLUDEPATH += $$PWD/../NodeAttributeControl
INCLUDEPATH += $$PWD/../Utils
INCLUDEPATH += $$PWD/../AudioVideoCore
INCLUDEPATH += $$PWD/../PythonWrap
INCLUDEPATH += $$PWD/../EasyCanvasCore
INCLUDEPATH += $$PWD/UICanvas
INCLUDEPATH += $$PWD/UICore
INCLUDEPATH += $$PWD/../thirdLibs/ffmpeg/include

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
    GlobalHotKey/GlobalHotKeyFilter.cpp \
    GlobalHotKey/GlobalHotKeyInfo.cpp \
    GlobalHotKey/GlobalHotKeyManager.cpp \
        UICore/MainWindow.cpp \
    UICore/UIAboutMeDialog.cpp \
        UICore/UIAttributWidget.cpp \
		main.cpp

HEADERS += \
        GlobalHotKey/GlobalHotKeyFilter.h \
        GlobalHotKey/GlobalHotKeyInfo.h \
        GlobalHotKey/GlobalHotKeyManager.h \
        UICore/MainWindow.h \
        UICore/UIAboutMeDialog.h \
        UICore/UIAttributWidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
