#include "UICanvasOperators.h"
#include "UICanvasView.h"
#include "UICanvasPathItem.h"
#include "UICanvas/UICanvasItemManager.h"
#include <QMouseEvent>

UICanvasOperBase::UICanvasOperBase(UICanvasView* view)
    :m_pCanvasView(view)
{

}

UICanvasOperBase::~UICanvasOperBase()
{

}

bool UICanvasOperBase::disposePressEvent(QMouseEvent* event)
{
    return false;
}

bool UICanvasOperBase::disposeMoveEvent(QMouseEvent* event)
{
    return false;
}

bool UICanvasOperBase::disposeReleaseEvent(QMouseEvent* event)
{
    m_pCanvasView->setCurrentOperator(new UICanvasDefaultOper(m_pCanvasView));
    return false;
}

bool UICanvasOperBase::disposeKeyPressEvent(QKeyEvent* event)
{
    return false;
}

// ---------------------------------------------------------------------
// 默认操作器，操作器调度器
UICanvasDefaultOper::UICanvasDefaultOper(UICanvasView* view)
    :UICanvasOperBase(view)
{

}

UICanvasDefaultOper::~UICanvasDefaultOper()
{

}

bool UICanvasDefaultOper::disposePressEvent(QMouseEvent* event)
{
    auto selectedItems = m_pCanvasView->scene()->selectedItems();
    auto items = m_pCanvasView->items(event->pos());

    UICanvasView::CanvasMode mode = m_pCanvasView->getCurrentMode();
    if (mode == UICanvasView::t_FreeDrawMode)
    {
        UICanvasPenOper* penFreeDrawOper = new UICanvasPenOper(m_pCanvasView);
        m_pCanvasView->setCurrentOperator(penFreeDrawOper);
        return penFreeDrawOper->disposePressEvent(event);
    }

    if (selectedItems.size() > 1 && items.size() > 0)
    {
        // 设置为多选移动操作器(选中的个数>1 且 当前鼠标下有Item时)
        UICanvasMoveSelectedItem* moveSelectOper = new UICanvasMoveSelectedItem(m_pCanvasView);
        m_pCanvasView->setCurrentOperator(moveSelectOper);
        return moveSelectOper->disposePressEvent(event);
    }
    else if (items.size() <= 0)
    {
        if (mode == UICanvasView::t_ArrowMode)
        {
            // 设置为矩形选择操作器（当前点击的为空白）
            UICanvasRectSelectOper* rectSelectOper = new UICanvasRectSelectOper(m_pCanvasView);
            m_pCanvasView->setCurrentOperator(rectSelectOper);
            return rectSelectOper->disposePressEvent(event);
        }
        else if (mode == UICanvasView::t_FreeDrawMode)
        {
            UICanvasPenOper* penFreeDrawOper = new UICanvasPenOper(m_pCanvasView);
            m_pCanvasView->setCurrentOperator(penFreeDrawOper);
            return penFreeDrawOper->disposePressEvent(event);
        }
    }

    return true;
}

bool UICanvasDefaultOper::disposeMoveEvent(QMouseEvent* event)
{
    return false;
}

bool UICanvasDefaultOper::disposeReleaseEvent(QMouseEvent* event)
{
    return false;
}

bool UICanvasDefaultOper::disposeKeyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Delete)
    {
        // 删除选中的元素
        auto selectedItems = m_pCanvasView->scene()->selectedItems();
        for (auto item : selectedItems)
            delete item;
    }

    return false;
}

// ---------------------------------------------------------------------
// 画笔操作器
UICanvasPenOper::UICanvasPenOper(UICanvasView* view)
    :UICanvasOperBase(view)
{
    UICanvasItemBase* item = g_currentCanvasManager->createCanvasItem(UICanvasItemManager::t_PathItem);
    m_pPathItem = dynamic_cast<UICanvasPathItem*>(item);
    view->scene()->addItem(m_pPathItem);
}

UICanvasPenOper::~UICanvasPenOper()
{

}

bool UICanvasPenOper::disposePressEvent(QMouseEvent* event)
{
    QPointF scenePos = m_pCanvasView->mapToScene(event->pos());
    m_pPathItem->setStartPos(scenePos);

    return UICanvasOperBase::disposePressEvent(event);
}

bool UICanvasPenOper::disposeMoveEvent(QMouseEvent* event)
{
    QPointF scenePos = m_pCanvasView->mapToScene(event->pos());
    m_pPathItem->addPoint(scenePos);

    return UICanvasOperBase::disposeMoveEvent(event);
}

bool UICanvasPenOper::disposeReleaseEvent(QMouseEvent* event)
{
    m_pPathItem->endDrawPos();

    m_pCanvasView->cleanAllSelected();
    m_pPathItem->setSelected(true);

    return UICanvasOperBase::disposeReleaseEvent(event);
}

// ---------------------------------------------------------------------
// 矩形选择操作器
UICanvasRectSelectOper::UICanvasRectSelectOper(UICanvasView* view)
    :UICanvasOperBase(view)
{

}

UICanvasRectSelectOper::~UICanvasRectSelectOper()
{

}

bool UICanvasRectSelectOper::disposePressEvent(QMouseEvent* event)
{
    // 设置为显示选中矩形
    m_pCanvasView->setSelectedRectVisible(true);
    m_pCanvasView->setSelectedRect(QRect());

    // 清除所有选中
    auto items = m_pCanvasView->scene()->selectedItems();
    foreach(auto item, items)
        item->setSelected(false);

    m_pressedViewPos = event->pos();
    m_pressedScenePos = m_pCanvasView->mapToScene(event->pos());

    return UICanvasOperBase::disposePressEvent(event);
}

bool UICanvasRectSelectOper::disposeMoveEvent(QMouseEvent* event)
{
    QPoint endPos = event->pos();
    m_pCanvasView->setSelectedRect(QRect(m_pressedViewPos, endPos));

    return UICanvasOperBase::disposeMoveEvent(event);
}

bool UICanvasRectSelectOper::disposeReleaseEvent(QMouseEvent* event)
{
    // 选中被选择的矩形
    QPointF endPos = m_pCanvasView->mapToScene(event->pos());
    qreal xStart = qMin(m_pressedScenePos.x(), endPos.x());
    qreal xEnd = qMax(m_pressedScenePos.x(), endPos.x());
    qreal yStart = qMin(m_pressedScenePos.y(), endPos.y());
    qreal yEnd = qMax(m_pressedScenePos.y(), endPos.y());

    //设置矩形值
    QRectF rect(QPointF(xStart, yStart), QPointF(xEnd, yEnd));
    QList<QGraphicsItem *> items = m_pCanvasView->scene()->items(rect, Qt::IntersectsItemShape);
    foreach(auto item, items)
        item->setSelected(true);

    m_pCanvasView->setSelectedRectVisible(false);

    return UICanvasOperBase::disposeReleaseEvent(event);
}

// ---------------------------------------------------------------------
// 多选移动操作器
UICanvasMoveSelectedItem::UICanvasMoveSelectedItem(UICanvasView* view)
    :UICanvasOperBase(view)
{

}

UICanvasMoveSelectedItem::~UICanvasMoveSelectedItem()
{

}

bool UICanvasMoveSelectedItem::disposePressEvent(QMouseEvent* event)
{
    m_items = m_pCanvasView->scene()->selectedItems();
    m_scenePos = m_pCanvasView->mapToScene(event->pos());

    return UICanvasOperBase::disposePressEvent(event);
}

bool UICanvasMoveSelectedItem::disposeMoveEvent(QMouseEvent* event)
{
    QPointF pos = m_pCanvasView->mapToScene(event->pos());
    qreal xIntervalue = pos.x() - m_scenePos.x();
    qreal yIntervalue = pos.y() - m_scenePos.y();
    m_scenePos = pos;

    // 重新设置全部选中的坐标
    foreach (auto item, m_items)
    {
        QPointF itemPos = item->pos();
        itemPos.setX(itemPos.x() + xIntervalue);
        itemPos.setY(itemPos.y() + yIntervalue);

        item->setPos(itemPos);
    }

    return UICanvasOperBase::disposeMoveEvent(event);
}

bool UICanvasMoveSelectedItem::disposeReleaseEvent(QMouseEvent* event)
{
    return UICanvasOperBase::disposeReleaseEvent(event);
}
