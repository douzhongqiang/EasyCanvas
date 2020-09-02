#include "MainWindow.h"
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGraphicsTextItem>
#include <QDockWidget>
#include <QToolBar>
#include <QToolButton>
#include <QMenu>
#include <QStatusBar>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QSharedPointer>
#include <QInputDialog>
#include <QDesktopServices>
#include "NDNodeBase.h"
#include "UIAboutMeDialog.h"
#include "PythonWrapCore.h"
#include "UICanvas/UICanvasItemManager.h"
#include "Scheme/CanvasSchemeManager.h"
#include "Scheme/UISchemeManagerWidget.h"
#include "Scheme/UISchemeManagerDialog.h"
#include "UndoCmd/UndoCmdCore.h"
#include "GlobalHotKey/GlobalHotKeyManager.h"
#include "GlobalHotKey/GlobalHotKeyInfo.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 创建画布
    m_pCanvasView = new UICanvasView;
    this->setCentralWidget(m_pCanvasView);
    g_PythonWrapCode->setSceneNode(m_pCanvasView->getCurrentSceneNode());

    // 添加属性界面
    m_pAttributeWidget = new UIAttributeWidget;
    m_pAttributeDockWidget = new QDockWidget(tr("Attribute"));
    m_pAttributeDockWidget->setWidget(m_pAttributeWidget);
    this->addDockWidget(Qt::RightDockWidgetArea, m_pAttributeDockWidget);

    // Undo View
    m_pUndoViewDockWidget = new QDockWidget(tr("UndoStack"));
    m_pUndoViewDockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
    m_pUndoViewDockWidget->setFloating(true);
    m_pUndoView = g_currentCanvasManager->getCurrentUndoCmdCore()->createUndoView();
    m_pUndoViewDockWidget->setWidget(m_pUndoView);
    this->addDockWidget(Qt::BottomDockWidgetArea, m_pUndoViewDockWidget);

    // 脚本编辑器
    QWidget* pWidget = createScriptConsoleWidget();
    m_pScriptConsoleDockWidget = new QDockWidget(tr("Script Console"));
    m_pScriptConsoleDockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
    m_pScriptConsoleDockWidget->setFloating(true);
    this->addDockWidget(Qt::BottomDockWidgetArea, m_pScriptConsoleDockWidget);
    m_pScriptConsoleDockWidget->setWidget(pWidget);
    this->splitDockWidget(m_pScriptConsoleDockWidget, m_pUndoViewDockWidget, Qt::Horizontal);

    // 添加节点树
    m_pNodeTreeWidget = new UINodeTreeWidget;
    m_pNodeTreeDockWidget = new QDockWidget(tr("NodeTree"));
    m_pNodeTreeDockWidget->setWidget(m_pNodeTreeWidget);
    this->addDockWidget(Qt::LeftDockWidgetArea, m_pNodeTreeDockWidget);

    QObject::connect(m_pCanvasView, &UICanvasView::itemSelectedChanged, \
                     this, &MainWindow::onSelectedItemChanged);

    initToolBar();
    initMenuBar();
    initStatusBar();

    // 初始化属性面板
    m_pAttributeWidget->setCurrentAttrNode(m_pCanvasView->getCurrentSelectedNode());
    // 初始化标题
    QString str("%1 - EasyCanvas");
    this->setWindowTitle(str.arg("Empty"));
}

MainWindow::~MainWindow()
{

}

QWidget* MainWindow::createScriptConsoleWidget(void)
{
    m_pScriptConsole = g_PythonWrapCode->getScriptConsole();
    QWidget* pScriptWidget = new QWidget;

    QVBoxLayout* mainLayout = new QVBoxLayout(pScriptWidget);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    QMenuBar* pMenuBar = new QMenuBar;

    mainLayout->addWidget(pMenuBar);
    mainLayout->addWidget(m_pScriptConsole);

    // 添加菜单
    QMenu *pFileMenu = new QMenu(tr("File"));
    pMenuBar->addMenu(pFileMenu);
    QAction* pFileAction = new QAction(tr("Open Script File"));
    QObject::connect(pFileAction, &QAction::triggered, this, &MainWindow::onClickedOpenScript);
    pFileMenu->addAction(pFileAction);

    QMenu* pHelpMenu = new QMenu(tr("Help"));
    pMenuBar->addMenu(pHelpMenu);
    QAction* pHelpAction = new QAction(tr("Script Help"));
    QObject::connect(pHelpAction, &QAction::triggered, this, &MainWindow::onClickedHelpScript);
    pHelpMenu->addAction(pHelpAction);

    return pScriptWidget;
}

void MainWindow::initGlobalHotKey(void)
{
    GlobalHotKeyManager* manager = new GlobalHotKeyManager(this);

    // Ctrl + Z 撤销
    GlobalHotKeyInfo* pInfoUndo = GlobalHotKeyInfo::createInstance(Qt::ControlModifier, Qt::Key_Z);
    manager->registerHotKey(QSharedPointer<GlobalHotKeyInfo>(pInfoUndo));
    QObject::connect(pInfoUndo, &GlobalHotKeyInfo::actived, this, &MainWindow::onUndoCmd);

    // Ctrl + Y 重做
    GlobalHotKeyInfo* pInfoRedo = GlobalHotKeyInfo::createInstance(Qt::ControlModifier, Qt::Key_Y);
    manager->registerHotKey(QSharedPointer<GlobalHotKeyInfo>(pInfoRedo));
    QObject::connect(pInfoRedo, &GlobalHotKeyInfo::actived, this, &MainWindow::onRedoCmd);

    // Ctrl + N 新建空文档
    GlobalHotKeyInfo* pInfoNewEmpty = GlobalHotKeyInfo::createInstance(Qt::ControlModifier, Qt::Key_N);
    manager->registerHotKey(QSharedPointer<GlobalHotKeyInfo>(pInfoNewEmpty));
    QObject::connect(pInfoNewEmpty, &GlobalHotKeyInfo::actived, this, &MainWindow::onClickedNew);

    // Ctrl + O 打开
    GlobalHotKeyInfo* pInfoOpenScheme = GlobalHotKeyInfo::createInstance(Qt::ControlModifier, Qt::Key_O);
    manager->registerHotKey(QSharedPointer<GlobalHotKeyInfo>(pInfoOpenScheme));
    QObject::connect(pInfoOpenScheme, &GlobalHotKeyInfo::actived, this, &MainWindow::onClickedOpen);

    // Ctrl + S 保存
    GlobalHotKeyInfo* pInfoSaveScheme = GlobalHotKeyInfo::createInstance(Qt::ControlModifier, Qt::Key_S);
    manager->registerHotKey(QSharedPointer<GlobalHotKeyInfo>(pInfoSaveScheme));
    QObject::connect(pInfoSaveScheme, &GlobalHotKeyInfo::actived, this, &MainWindow::onClickedSava);
}

void MainWindow::onSelectedItemChanged(void)
{
    NDNodeBase* node = m_pCanvasView->getCurrentSelectedNode();
    m_pAttributeWidget->setCurrentAttrNode(node);
}

void MainWindow::initToolBar(void)
{
    // 添加工具栏
    m_pToolBar = new QToolBar;
    this->addToolBar(m_pToolBar);

    QAction* pArrowDrawButton = new QAction(QIcon("./images/arrowItem.png"), tr("Select"));
    pArrowDrawButton->setCheckable(true);
    pArrowDrawButton->setChecked(true);
    QObject::connect(pArrowDrawButton, &QAction::triggered, this, &MainWindow::onClickedArrowButton);
    m_pToolBar->addAction(pArrowDrawButton);

    QAction* pFreedowDrawButton = new QAction(QIcon("./images/freeDrawItem.png"), tr("Pen Item"));
    pFreedowDrawButton->setCheckable(true);
    QObject::connect(pFreedowDrawButton, &QAction::triggered, this, &MainWindow::onClickedFreeDrawButton);
    m_pToolBar->addAction(pFreedowDrawButton);

    QAction* pImageButton = new QAction(QIcon("./images/imageItem.png"), tr("Image Item"));
    QObject::connect(pImageButton, &QAction::triggered, this, &MainWindow::onClickedImageButton);
    m_pToolBar->addAction(pImageButton);

    QAction* pEllipseButton = new QAction(QIcon("./images/ellipseItem.png"), tr("Ellipse Item"));
    QObject::connect(pEllipseButton, &QAction::triggered, this, &MainWindow::onClickedEllipseButton);
    m_pToolBar->addAction(pEllipseButton);

    QAction* pRectButton = new QAction(QIcon("./images/rectItem.png"), tr("Rect Item"));
    QObject::connect(pRectButton, &QAction::triggered, this, &MainWindow::onClickedRectButton);
    m_pToolBar->addAction(pRectButton);

    QAction* pTextButton = new QAction(QIcon("./images/textItem.png"), tr("Text Item"));
    QObject::connect(pTextButton, &QAction::triggered, this, &MainWindow::onClickedTextButton);
    m_pToolBar->addAction(pTextButton);

    QAction* pAudioButton = new QAction(QIcon("./images/audioItem.png"), tr("Audio Item"));
    QObject::connect(pAudioButton, &QAction::triggered, this, &MainWindow::onClickedAudioButton);
    m_pToolBar->addAction(pAudioButton);
}

void MainWindow::initMenuBar(void)
{
    QMenuBar* menuBar = new QMenuBar;
    this->setMenuBar(menuBar);

    // 添加文件菜单
    QMenu* fileMenu = new QMenu(tr("File"));
    menuBar->addMenu(fileMenu);
    // 新建
    QAction* newAction = new QAction(tr("New"));
    newAction->setShortcut(Qt::CTRL + Qt::Key_N);
    fileMenu->addAction(newAction);
    QObject::connect(newAction, &QAction::triggered, this, &MainWindow::onClickedNew);
    // 打开
    QAction* openAction = new QAction(tr("Open"));
    openAction->setShortcut(Qt::CTRL + Qt::Key_O);
    fileMenu->addAction(openAction);
    QObject::connect(openAction, &QAction::triggered, this, &MainWindow::onClickedOpen);
    // 保存
    QAction* saveAction = new QAction(tr("Save"));
    saveAction->setShortcut(Qt::CTRL + Qt::Key_S);
    fileMenu->addAction(saveAction);
    QObject::connect(saveAction, &QAction::triggered, this, &MainWindow::onClickedSava);
    // 另存为
    QAction* saveAsAction = new QAction(tr("SaveAs"));
    fileMenu->addAction(saveAsAction);
    QObject::connect(saveAsAction, &QAction::triggered, this, &MainWindow::onClickedSaveAs);
    // 导出为图片
    QAction* saveImageAction = new QAction(tr("Save To Image"));
    fileMenu->addAction(saveImageAction);
    QObject::connect(saveImageAction, &QAction::triggered, this, &MainWindow::onClickedSaveToImage);
    fileMenu->addSeparator();
    // 打开脚本文件
    QAction* pFileAction = new QAction(tr("Open Script File"));
    QObject::connect(pFileAction, &QAction::triggered, this, &MainWindow::onClickedOpenScript);
    fileMenu->addAction(pFileAction);

    // 添加编辑菜单
    QMenu* editMenu = new QMenu(tr("Edit"));
    menuBar->addMenu(editMenu);
    // 脚本编辑器
    QAction* pythonScriptAction = new QAction(tr("Script Editor"));
    editMenu->addAction(pythonScriptAction);
    QObject::connect(pythonScriptAction, &QAction::triggered, this, &MainWindow::onClickedScriptEdit);
    editMenu->addSeparator();
    // 撤销
    QAction* undoAction = g_currentCanvasManager->getCurrentUndoCmdCore()->createUndoAction();
    undoAction->setShortcut(Qt::CTRL + Qt::Key_Z);
    editMenu->addAction(undoAction);
    // 重做
    QAction* redoAction = g_currentCanvasManager->getCurrentUndoCmdCore()->createRedoAction();
    redoAction->setShortcut(Qt::CTRL + Qt::Key_Y);
    editMenu->addAction(redoAction);
    // UndoView
    QAction* undoViewAction = new QAction(tr("Undo View"));
    QObject::connect(undoViewAction, &QAction::triggered, this, &MainWindow::onClickedUndoView);
    editMenu->addAction(undoViewAction);

    // 添加Item
    QMenu* canvasItem = new QMenu(tr("CanvasItem"));
    menuBar->addMenu(canvasItem);

    QAction* pArrowDrawButton = new QAction(QIcon("./images/arrowItem.png"), tr("Select"));
    QObject::connect(pArrowDrawButton, &QAction::triggered, this, &MainWindow::onClickedArrowButton);
    canvasItem->addAction(pArrowDrawButton);

    QAction* pFreedowDrawButton = new QAction(QIcon("./images/freeDrawItem.png"), tr("Pen Item"));
    QObject::connect(pFreedowDrawButton, &QAction::triggered, this, &MainWindow::onClickedFreeDrawButton);
    canvasItem->addAction(pFreedowDrawButton);

    QAction* pImageButton = new QAction(QIcon("./images/imageItem.png"), tr("Image Item"));
    QObject::connect(pImageButton, &QAction::triggered, this, &MainWindow::onClickedImageButton);
    canvasItem->addAction(pImageButton);

    QAction* pEllipseButton = new QAction(QIcon("./images/ellipseItem.png"), tr("Ellipse Item"));
    QObject::connect(pEllipseButton, &QAction::triggered, this, &MainWindow::onClickedEllipseButton);
    canvasItem->addAction(pEllipseButton);

    QAction* pRectButton = new QAction(QIcon("./images/rectItem.png"), tr("Rect Item"));
    QObject::connect(pRectButton, &QAction::triggered, this, &MainWindow::onClickedRectButton);
    canvasItem->addAction(pRectButton);

    QAction* pTextButton = new QAction(QIcon("./images/textItem.png"), tr("Text Item"));
    QObject::connect(pTextButton, &QAction::triggered, this, &MainWindow::onClickedTextButton);
    canvasItem->addAction(pTextButton);

    QAction* pAudioButton = new QAction(QIcon("./images/audioItem.png"), tr("Audio Item"));
    QObject::connect(pAudioButton, &QAction::triggered, this, &MainWindow::onClickedAudioButton);
    canvasItem->addAction(pAudioButton);

    // 添加帮助
    QMenu* helpMenu = new QMenu(tr("Help"));
    menuBar->addMenu(helpMenu);
    // 关于我
    QAction* pHelpAction = new QAction(tr("About Me"));
    QObject::connect(pHelpAction, &QAction::triggered, this, &MainWindow::onClickedAboutMe);
    helpMenu->addAction(pHelpAction);
    // 脚本命令
    QAction* pScriptHelpAction = new QAction(tr("Script Help"));
    QObject::connect(pScriptHelpAction, &QAction::triggered, this, &MainWindow::onClickedHelpScript);
    helpMenu->addAction(pScriptHelpAction);
}

void MainWindow::initStatusBar(void)
{
    QStatusBar* statusBar = new QStatusBar;
    this->setStatusBar(statusBar);

    statusBar->showMessage(tr("Ready"));
}

void MainWindow::onClickedImageButton(void)
{
    m_pCanvasView->createImageItem();
}

void MainWindow::onClickedArrowButton(void)
{
    m_pCanvasView->setCurrentMode(UICanvasView::t_ArrowMode);
}

void MainWindow::onClickedFreeDrawButton(void)
{
    m_pCanvasView->setCurrentMode(UICanvasView::t_FreeDrawMode);
}

void MainWindow::onClickedRectButton(void)
{
    m_pCanvasView->createRectItem();
}

void MainWindow::onClickedEllipseButton(void)
{
    m_pCanvasView->createEllipseItem();
}

void MainWindow::onClickedAudioButton(void)
{
    m_pCanvasView->createAudioItem();
}

void MainWindow::onClickedTextButton(void)
{
    m_pCanvasView->createTextItem();
}

void MainWindow::onClickedSaveToImage(void)
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), "./EasyCanvas.png", tr("Images (*.png *.jpg *.bmp)"));
    if (fileName.isEmpty())
        return;

    m_pCanvasView->saveToImage(fileName);

    QMessageBox message;
    message.about(this, tr("Save Info"), tr("Save Successed!"));
}

void MainWindow::onClickedOpenScript(void)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Script File"), "./", tr("Python (*.py)"));
    if (fileName.isEmpty())
        return;

    g_PythonWrapCode->runScriptFiles(fileName);
}

void MainWindow::onClickedNew(void)
{
    g_CanvasSchemeManager->setCurrentSchemeName("");
    g_currentCanvasManager->cleanAll();

    QString str("%1 - EasyCanvas");
    this->setWindowTitle(str.arg("Empty"));
}

void MainWindow::onClickedOpen(void)
{
    this->statusBar()->showMessage(tr("Opening..."));

    UISchemeManagerDialog pSchemeManagerWidget(this);
    pSchemeManagerWidget.exec();

    this->statusBar()->showMessage(tr("Opened Successed"));

    // 设置标题
    QString str("%1 - EasyCanvas");
    this->setWindowTitle(str.arg(g_CanvasSchemeManager->getCurrentSchemeName()));
}

void MainWindow::onClickedSava(void)
{
    if (g_CanvasSchemeManager->getCurrentSchemeName().isEmpty())
        onClickedSaveAs();
    else {
        this->statusBar()->showMessage(tr("Saving..."));
        g_CanvasSchemeManager->saveScheme();
        this->statusBar()->showMessage(tr("Save Successed"));
    }

    // 设置标题
    QString str("%1 - EasyCanvas");
    this->setWindowTitle(str.arg(g_CanvasSchemeManager->getCurrentSchemeName()));
}

void MainWindow::onClickedSaveAs(void)
{
    QString schemeName = QInputDialog::getText(this, tr("Please Input Scheme Name"), tr("Please Input Scheme Name"));
    if (schemeName.isEmpty())
        return;

    g_CanvasSchemeManager->setCurrentSchemeName(schemeName);
    this->statusBar()->showMessage(tr("Saving..."));
    g_CanvasSchemeManager->saveScheme();
    this->statusBar()->showMessage(tr("Save Successed"));

    // 设置标题
    QString str("%1 - EasyCanvas");
    this->setWindowTitle(str.arg(g_CanvasSchemeManager->getCurrentSchemeName()));
}

void MainWindow::onClickedAboutMe(void)
{
    UIAboutMeDialog aboutMeDialog;
    aboutMeDialog.resize(900, 600);
    aboutMeDialog.exec();
}

void MainWindow::onClickedHelpScript(void)
{
    QDesktopServices::openUrl(QUrl("https://www.douzhq.cn/easycanvas_cmd/"));
}

void MainWindow::onClickedScriptEdit(void)
{

    m_pScriptConsoleDockWidget->show();
}

// 撤销
void MainWindow::onUndoCmd(void)
{
    g_currentCanvasManager->undo();
}

// 重做
void MainWindow::onRedoCmd(void)
{
    g_currentCanvasManager->redo();
}

void MainWindow::onClickedUndoView(void)
{
    m_pUndoViewDockWidget->show();
}
