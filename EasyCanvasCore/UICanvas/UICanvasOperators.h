#ifndef UICANVASOPERATORS_H
#define UICANVASOPERATORS_H

#include <QPointF>
#include <QGraphicsItem>
class UICanvasView;
class QMouseEvent;
class UICanvasPathItem;
class NDAttributeBase;
class UICanvasOperBase
{
public:
    UICanvasOperBase(UICanvasView* view);
    virtual ~UICanvasOperBase();

    virtual bool disposePressEvent(QMouseEvent* event);
    virtual bool disposeMoveEvent(QMouseEvent* event);
    virtual bool disposeReleaseEvent(QMouseEvent* event);
    virtual bool disposeKeyPressEvent(QKeyEvent* event);

protected:
    UICanvasView* m_pCanvasView = nullptr;
};

// ---------------------------------------------------------------------
// 默认操作器，操作器调度器
class UICanvasDefaultOper : public UICanvasOperBase
{
public:
    UICanvasDefaultOper(UICanvasView* view);
    ~UICanvasDefaultOper();

    bool disposePressEvent(QMouseEvent* event) override;
    bool disposeMoveEvent(QMouseEvent* event) override;
    bool disposeReleaseEvent(QMouseEvent* event) override;

    bool disposeKeyPressEvent(QKeyEvent* event) override;
};

// ---------------------------------------------------------------------
// 画笔操作器(自由绘制)
class UICanvasPenOper : public UICanvasOperBase
{
public:
    UICanvasPenOper(UICanvasView* view);
    ~UICanvasPenOper();

    bool disposePressEvent(QMouseEvent* event) override;
    bool disposeMoveEvent(QMouseEvent* event) override;
    bool disposeReleaseEvent(QMouseEvent* event) override;

private:
    UICanvasPathItem* m_pPathItem = nullptr;
};

// ---------------------------------------------------------------------
// 矩形选择操作器
class UICanvasRectSelectOper : public UICanvasOperBase
{
public:
    UICanvasRectSelectOper(UICanvasView* view);
    ~UICanvasRectSelectOper();

    bool disposePressEvent(QMouseEvent* event) override;
    bool disposeMoveEvent(QMouseEvent* event) override;
    bool disposeReleaseEvent(QMouseEvent* event) override;

private:
    QPoint m_pressedViewPos;
    QPointF m_pressedScenePos;
};

// ---------------------------------------------------------------------
// 多选移动操作器
class UICanvasMoveSelectedItem : public UICanvasOperBase
{
public:
    UICanvasMoveSelectedItem(UICanvasView* view);
    ~UICanvasMoveSelectedItem();

    bool disposePressEvent(QMouseEvent* event) override;
    bool disposeMoveEvent(QMouseEvent* event) override;
    bool disposeReleaseEvent(QMouseEvent* event) override;

private:
    QPointF m_scenePos;
    QList<QGraphicsItem *> m_items;

    QList<NDAttributeBase*> m_xAttributes;
    QList<NDAttributeBase*> m_yAttributes;
    QVector<QVariant> m_xValues;
    QVector<QVariant> m_yValues;

    QVector<QVariant> m_xOldValues;
    QVector<QVariant> m_yOldValues;
};

// ---------------------------------------------------------------------
// 删除操作器
class UICanvasDeleteOperator : public UICanvasOperBase
{
public:

};

#endif
