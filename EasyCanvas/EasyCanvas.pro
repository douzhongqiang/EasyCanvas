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
DESTDIR += $$PWD/bin

LIBS += -L$$PWD/../lib/CustomWidgets -lCustomWidgets
LIBS += -L$$PWD/../lib/CustomControls -lCustomControls
LIBS += -L$$PWD/../lib/Utils -lUtils
LIBS += -L$$PWD/../lib/NDNodeAttribute -lNDNodeAttribute
LIBS += -L$$PWD/../lib/NodeAttributeControl -lNodeAttributeControl
LIBS += -L$$PWD/../lib/AudioVideoCore -lAudioVideoCore

INCLUDEPATH += $$PWD/../CustomWidgets
INCLUDEPATH += $$PWD/../CustomControls
INCLUDEPATH += $$PWD/../NDNodeAttribute
INCLUDEPATH += $$PWD/../NodeAttributeControl
INCLUDEPATH += $$PWD/../Utils
INCLUDEPATH += $$PWD/../AudioVideoCore
INCLUDEPATH += $$PWD/UICanvas
INCLUDEPATH += $$PWD/UICore
INCLUDEPATH += $$PWD/../thirdLibs/ffmpeg/include

SOURCES += \
        UICanvas/UICanvasArrows.cpp \
    UICanvas/UICanvasAudioItem.cpp \
        UICanvas/UICanvasEllipseItem.cpp \
        UICanvas/UICanvasImageItem.cpp \
        UICanvas/UICanvasItemBase.cpp \
        UICanvas/UICanvasOperators.cpp \
        UICanvas/UICanvasPathItem.cpp \
        UICanvas/UICanvasRectItem.cpp \
    UICanvas/UICanvasScene.cpp \
        UICanvas/UICanvasTextItem.cpp \
        UICanvas/UICanvasView.cpp \
        UICore/MainWindow.cpp \
    UICore/UIAboutMeDialog.cpp \
        UICore/UIAttributWidget.cpp \
		main.cpp

HEADERS += \
        UICanvas/UICanvasArrows.h \
        UICanvas/UICanvasAudioItem.h \
        UICanvas/UICanvasEllipseItem.h \
        UICanvas/UICanvasImageItem.h \
        UICanvas/UICanvasItemBase.h \
        UICanvas/UICanvasOperators.h \
        UICanvas/UICanvasPathItem.h \
        UICanvas/UICanvasRectItem.h \
        UICanvas/UICanvasScene.h \
        UICanvas/UICanvasTextItem.h \
        UICanvas/UICanvasView.h \
        UICore/MainWindow.h \
        UICore/UIAboutMeDialog.h \
        UICore/UIAttributWidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
