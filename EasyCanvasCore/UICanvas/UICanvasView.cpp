#include "UICanvasView.h"
#include "UICanvasImageItem.h"
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include "UICanvasPathItem.h"
#include "UICanvasOperators.h"
#include "UICanvasTextItem.h"
#include "UICanvasRectItem.h"
#include "UICanvasEllipseItem.h"
#include "UICanvasAudioItem.h"
#include "UICanvasScene.h"
#include "UICanvasItemManager.h"
#include <QApplication>

UICanvasView::UICanvasView(QWidget* parent)
    :QGraphicsView(parent)
    , m_pCurrentOper(nullptr)
    , m_cSelectedPenColor(200, 100, 100)
    , m_cSelectedBrushColor(0, 0, 200, 100)
{
    // 初始化CanvasManager
    UICanvasItemManager::createCanvasManager();
    UICanvasItemManager::setCurrentIndex(0);
    g_currentCanvasManager->setCurrentCanvasView(this);

    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    m_pScene = new UICanvasScene;
    QObject::connect(m_pScene, &UICanvasScene::selectionChanged, \
        this, &UICanvasView::itemSelectedChanged);

    this->setScene(m_pScene);
    //this->setSceneRect(0, 0, 5000, 5000);

    // 设置默认操作器
    this->setCurrentOperator(new UICanvasDefaultOper(this));

    m_penPixmap = QPixmap();
    QString curAppPath = qApp->applicationDirPath() + "/";
    m_penPixmap.load(curAppPath + "./images/freeDrawItem.png");
}

UICanvasView::~UICanvasView()
{

}

void UICanvasView::createImageItem(void)
{
    g_currentCanvasManager->createCanvasItemByCmd(UICanvasItemManager::t_ImageItem);
}

void UICanvasView::createTextItem(void)
{
    g_currentCanvasManager->createCanvasItemByCmd(UICanvasItemManager::t_TextItem);
}

void UICanvasView::createRectItem(void)
{
    g_currentCanvasManager->createCanvasItemByCmd(UICanvasItemManager::t_RectItem);
}

void UICanvasView::createEllipseItem(void)
{
    g_currentCanvasManager->createCanvasItemByCmd(UICanvasItemManager::t_EllipseItem);
}

void UICanvasView::createAudioItem(void)
{
    g_currentCanvasManager->createCanvasItemByCmd(UICanvasItemManager::t_AudioItem);
}

// 设置选中矩形
void UICanvasView::setSelectedRectVisible(bool isVisible)
{
    m_isSelectedRectVisible = isVisible;
    this->viewport()->update();
}

// 设置选中矩形范围
void UICanvasView::setSelectedRect(const QRect& rect)
{
    QPointF startPos = this->mapToScene(rect.topLeft());
    QPointF endPos = this->mapToScene(rect.bottomRight());
    m_selectedRect = QRect(QPoint(startPos.x(), startPos.y()), \
        QPoint(endPos.x(), endPos.y()));

    this->viewport()->update();
}

void UICanvasView::setCurrentOperator(UICanvasOperBase* canvasOper)
{
    m_pCurrentOper.reset(canvasOper);
}

void UICanvasView::mousePressEvent(QMouseEvent* event)
{
    bool result = m_pCurrentOper->disposePressEvent(event);
    if (result)
        QGraphicsView::mousePressEvent(event);
}

void UICanvasView::mouseMoveEvent(QMouseEvent* event)
{
    m_pCurrentOper->disposeMoveEvent(event);
    return QGraphicsView::mouseMoveEvent(event);
}

void UICanvasView::mouseReleaseEvent(QMouseEvent* event)
{
    m_pCurrentOper->disposeReleaseEvent(event);
    return QGraphicsView::mouseReleaseEvent(event);
}

void UICanvasView::keyPressEvent(QKeyEvent* event)
{
    m_pCurrentOper->disposeKeyPressEvent(event);
}

void UICanvasView::drawBackground(QPainter* painter, const QRectF& rect)
{
    return QGraphicsView::drawBackground(painter, rect);
}

void UICanvasView::drawForeground(QPainter* painter, const QRectF& rect)
{
    if (!m_isSelectedRectVisible)
        return QGraphicsView::drawForeground(painter, rect);

    QPen pen;
    pen.setWidth(1);
    pen.setColor(m_cSelectedPenColor);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);

    painter->setBrush(m_cSelectedBrushColor);
    painter->drawRect(m_selectedRect);

    return QGraphicsView::drawForeground(painter, rect);
}

// 设置/获取当前模式
void UICanvasView::setCurrentMode(CanvasMode mode)
{
    m_mode = mode;

    if (m_mode == t_FreeDrawMode && !m_penPixmap.isNull())
    {
        this->viewport()->setCursor(QCursor(m_penPixmap, 0, m_penPixmap.height()));
    }
    else {
        this->viewport()->setCursor(Qt::ArrowCursor);
    }
}

UICanvasView::CanvasMode UICanvasView::getCurrentMode(void)
{
    return m_mode;
}

NDNodeBase* UICanvasView::getCurrentSelectedNode(void)
{
    QList<QGraphicsItem*> items = m_pScene->selectedItems();
    if (items.size() == 1)
    {
        UICanvasItemBase* canvasItem = qgraphicsitem_cast<UICanvasItemBase*>(items[0]);
        if (canvasItem == nullptr)
            return nullptr;

        return canvasItem->getCurrentNode();
    }

    return m_pScene->getCurrentNode();
}

QList<NDNodeBase*> UICanvasView::getCurrentSelectedNodes(void)
{
    QList<NDNodeBase*> nodes;

    QList<QGraphicsItem*> items = m_pScene->selectedItems();
    for (auto iter = items.begin(); iter != items.end(); ++iter)
    {
        UICanvasItemBase* canvasItem = qgraphicsitem_cast<UICanvasItemBase*>(*iter);
        if (canvasItem)
            nodes << canvasItem->getCurrentNode();
    }

    return nodes;
}

QList<UICanvasItemBase*> UICanvasView::getCurrentSelectedItems(void)
{
    QList<UICanvasItemBase*> selectedItems;

    QList<QGraphicsItem*> items = m_pScene->selectedItems();
    for (auto iter = items.begin(); iter != items.end(); ++iter)
    {
        UICanvasItemBase* canvasItem = qgraphicsitem_cast<UICanvasItemBase*>(*iter);
        if (canvasItem)
            selectedItems << canvasItem;
    }

    return selectedItems;
}

NDNodeBase* UICanvasView::getCurrentSceneNode(void)
{
    return m_pScene->getCurrentNode();
}

void UICanvasView::resetSceneNode(void)
{
    m_pScene->resetNodeInfo();
}

QPointF UICanvasView::getCenterPos(void)
{
    QRectF rect = m_pScene->sceneRect();
    return rect.center();
}

void UICanvasView::saveToImage(const QString& imagePath)
{
    int width = m_pScene->width();
    int height = m_pScene->height();

    QImage image(QSize(width, height), QImage::Format_RGB32);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    cleanAllSelected();
    m_pScene->render(&painter);
    image.save(imagePath);
}

// 添加到场景中
void UICanvasView::addToScene(UICanvasItemBase* item)
{
    m_pScene->addItem(item);
}

// 从场景中删除
void UICanvasView::removeFromScene(UICanvasItemBase* item)
{
    m_pScene->removeItem(item);
}

void UICanvasView::saveToImage(QImage& saveImage)
{
    int width = m_pScene->width();
    int height = m_pScene->height();

    QImage image(QSize(width, height), QImage::Format_RGB32);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    cleanAllSelected();
    m_pScene->render(&painter);
    saveImage = image;
}

void UICanvasView::cleanAllSelected(void)
{
    auto items = m_pScene->selectedItems();
    foreach(auto item, items)
    {
        item->setSelected(false);
    }
}
