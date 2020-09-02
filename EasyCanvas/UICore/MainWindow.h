#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QUndoView>
#include "UICanvas/UICanvasView.h"
#include "UIAttributWidget.h"
#include "UINodeTree/UINodeTreeWidget.h"

class PythonQtScriptingConsole;
class QToolBar;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 注册全局热键
    void initGlobalHotKey(void);

private:
    UICanvasView* m_pCanvasView = nullptr;
    // 属性界面
    UIAttributeWidget* m_pAttributeWidget = nullptr;
    QDockWidget* m_pAttributeDockWidget = nullptr;
    // 节点树
    UINodeTreeWidget* m_pNodeTreeWidget = nullptr;
    QDockWidget* m_pNodeTreeDockWidget = nullptr;
    // Undo View
    QUndoView* m_pUndoView = nullptr;
    QDockWidget* m_pUndoViewDockWidget = nullptr;
    // 脚本编辑器
    QWidget* m_pScriptConsole = nullptr;
    QDockWidget* m_pScriptConsoleDockWidget = nullptr;

    QToolBar* m_pToolBar = nullptr;

    void initToolBar(void);
    void initMenuBar(void);
    void initStatusBar(void);

    QWidget* createScriptConsoleWidget(void);

private slots:
    void onClickedImageButton(void);
    void onClickedArrowButton(void);
    void onClickedFreeDrawButton(void);

    void onClickedRectButton(void);
    void onClickedEllipseButton(void);
    void onClickedAudioButton(void);
    void onClickedTextButton(void);

    void onSelectedItemChanged(void);

    // 文件相关
    void onClickedNew(void);                 // 新建场景
    void onClickedOpen(void);                // 打开场景
    void onClickedSava(void);                // 保存场景
    void onClickedSaveAs(void);              // 另存为
    void onClickedSaveToImage(void);         // 保存为图片
    void onClickedOpenScript(void);          // 打开脚本文件

    // 关于我
    void onClickedAboutMe(void);
    void onClickedHelpScript(void);

    // 编辑菜单
    // 脚本编辑器
    void onClickedScriptEdit(void);
    // 撤销
    void onUndoCmd(void);
    // 重做
    void onRedoCmd(void);
    // 显示Undo View
    void onClickedUndoView(void);
};

#endif // WIDGET_H
