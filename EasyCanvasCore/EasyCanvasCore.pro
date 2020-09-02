#-------------------------------------------------
#
# Project created by QtCreator 2020-08-03T18:18:55
#
#-------------------------------------------------

QT       += widgets multimedia sql

TARGET = EasyCanvasCore
TEMPLATE = lib

DEFINES += EASYCANVASCORE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

MOC_DIR += $$PWD/../EasyCanvasCore/temp
OBJECTS_DIR += $$PWD/../EasyCanvasCore/temp
DESTDIR += $$PWD/../bin

LIBS += -L$$PWD/../bin -lCustomWidgets
LIBS += -L$$PWD/../bin -lCustomControls
LIBS += -L$$PWD/../bin -lUtils
LIBS += -L$$PWD/../bin -lNDNodeAttribute
LIBS += -L$$PWD/../bin -lNodeAttributeControl
LIBS += -L$$PWD/../bin -lAudioVideoCore

INCLUDEPATH += $$PWD/../CustomWidgets
INCLUDEPATH += $$PWD/../CustomControls
INCLUDEPATH += $$PWD/../NDNodeAttribute
INCLUDEPATH += $$PWD/../NodeAttributeControl
INCLUDEPATH += $$PWD/../Utils
INCLUDEPATH += $$PWD/../AudioVideoCore
INCLUDEPATH += $$PWD/../thirdLibs/ffmpeg/include

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Scheme/CanvasSchemeManager.cpp \
        Scheme/SQLCore.cpp \
        Scheme/UISchemeManagerDialog.cpp \
        Scheme/UISchemeManagerWidget.cpp \
        UICanvas/UICanvasAudioItem.cpp \
        UICanvas/UICanvasEllipseItem.cpp \
        UICanvas/UICanvasImageItem.cpp \
        UICanvas/UICanvasItemBase.cpp \
        UICanvas/UICanvasItemManager.cpp \
        UICanvas/UICanvasOperators.cpp \
        UICanvas/UICanvasPathItem.cpp \
        UICanvas/UICanvasRectItem.cpp \
        UICanvas/UICanvasScene.cpp \
        UICanvas/UICanvasTextItem.cpp \
        UICanvas/UICanvasView.cpp \
        UINodeTree/UINodeItem.cpp \
        UINodeTree/UINodeSortFilterProxyModel.cpp \
        UINodeTree/UINodeTreeDelegate.cpp \
        UINodeTree/UINodeTreeView.cpp \
        UINodeTree/UINodeTreeViewModel.cpp \
        UINodeTree/UINodeTreeWidget.cpp \
        UndoCmd/ItemAttributeChangedCmd.cpp \
        UndoCmd/ItemChangeNameCmd.cpp \
        UndoCmd/ItemCreateCmd.cpp \
        UndoCmd/ItemDeleteCmd.cpp \
        UndoCmd/ItemPasteCmd.cpp \
        UndoCmd/UndoCmdCore.cpp

HEADERS += \
        Scheme/CanvasSchemeManager.h \
        Scheme/SQLCore.h \
        Scheme/SchemeDataInfo.h \
        Scheme/UISchemeManagerDialog.h \
        Scheme/UISchemeManagerWidget.h \
        UICanvas/UICanvasAudioItem.h \
        UICanvas/UICanvasEllipseItem.h \
        UICanvas/UICanvasImageItem.h \
        UICanvas/UICanvasItemBase.h \
        UICanvas/UICanvasItemManager.h \
        UICanvas/UICanvasOperators.h \
        UICanvas/UICanvasPathItem.h \
        UICanvas/UICanvasRectItem.h \
        UICanvas/UICanvasScene.h \
        UICanvas/UICanvasTextItem.h \
        UICanvas/UICanvasView.h \
        UINodeTree/UINodeItem.h \
        UINodeTree/UINodeSortFilterProxyModel.h \
        UINodeTree/UINodeTreeDelegate.h \
        UINodeTree/UINodeTreeView.h \
        UINodeTree/UINodeTreeViewModel.h \
        UINodeTree/UINodeTreeWidget.h \
        UndoCmd/ItemAttributeChangedCmd.h \
        UndoCmd/ItemChangeNameCmd.h \
        UndoCmd/ItemCreateCmd.h \
        UndoCmd/ItemDeleteCmd.h \
        UndoCmd/ItemPasteCmd.h \
        UndoCmd/UndoCmdCore.h \
        easycanvascore_global.h 

unix {
    target.path = /usr/lib
    INSTALLS += target
}
