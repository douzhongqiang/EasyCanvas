#include "MainWindow.h"
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGraphicsTextItem>
#include <QDockWidget>
#include <QToolBar>
#include <QToolButton>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include "NDNodeBase.h"
#include "UIAboutMeDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_pCanvasView = new UICanvasView;
    this->setCentralWidget(m_pCanvasView);

    // 添加属性界面
    m_pAttributeWidget = new UIAttributeWidget;
    QDockWidget* attrDockWidget = new QDockWidget(tr("Attribute"));
    attrDockWidget->setWidget(m_pAttributeWidget);
    this->addDockWidget(Qt::RightDockWidgetArea, attrDockWidget);

    QObject::connect(m_pCanvasView, &UICanvasView::itemSelectedChanged, \
                     this, &MainWindow::onSelectedItemChanged);

    initToolBar();
    initMenuBar();

    // 初始化属性面板
    m_pAttributeWidget->setCurrentAttrNode(m_pCanvasView->getCurrentSelectedNode());
}

MainWindow::~MainWindow()
{

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
    QAction* saveAction = new QAction(tr("Save As"));
    fileMenu->addAction(saveAction);
    QObject::connect(saveAction, &QAction::triggered, this, &MainWindow::onClickedSaveAs);

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

    QAction* pHelpAction = new QAction(tr("About Me"));
    QObject::connect(pHelpAction, &QAction::triggered, this, &MainWindow::onClickedAboutMe);
    helpMenu->addAction(pHelpAction);
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

void MainWindow::onClickedSaveAs(void)
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), "./EasyCanvas.png", tr("Images (*.png *.jpg *.bmp)"));
    if (fileName.isEmpty())
        return;

    m_pCanvasView->saveToImage(fileName);

    QMessageBox message;
    message.about(this, tr("Save Info"), tr("Save Successed!"));
}

void MainWindow::onClickedAboutMe(void)
{
    UIAboutMeDialog aboutMeDialog;
    aboutMeDialog.resize(900, 600);
    aboutMeDialog.exec();
}
