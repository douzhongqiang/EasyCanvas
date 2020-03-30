#include "UICanvasPathItem.h"
#include "NDIntAttribute.h"
#include "NDColorAttribute.h"
#include "NDNodeBase.h"
#include <QPainter>

UICanvasPathItem::UICanvasPathItem(QGraphicsItem* parentItem)
    :UICanvasItemBase(parentItem)
{
    this->setItemResizeable(false);
    initAttribute();
}

UICanvasPathItem::~UICanvasPathItem()
{

}

void UICanvasPathItem::addPoint(const QPointF& scenePos)
{
    QPointF localPos = this->mapFromScene(scenePos);
    m_painterPath.lineTo(localPos);

    this->update();
}

void UICanvasPathItem::customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QPen pen;
    pen.setWidth(m_pOutLineWidthAttribute->getCurrentValue());
    pen.setColor(m_pOutLineColorAttribute->getCurrentValue());
    painter->setPen(pen);

    painter->drawPath(m_painterPath);
    painter->restore();
}

void UICanvasPathItem::initAttribute(void)
{
    QString groupName = tr("Item Attribute");
    m_pNode->addAttributeGroup(groupName);

    // 轮廓线宽度
    m_pOutLineWidthAttribute = new NDIntAttribute;
    m_pOutLineWidthAttribute->setCurrentValue(1);
    m_pOutLineWidthAttribute->setValueRange(1, 20);
    m_pOutLineWidthAttribute->setDisplayName(tr("OutLine Width: "));
    m_pOutLineWidthAttribute->setName("outlineWidth");
    m_pNode->addAttribute(groupName, m_pOutLineWidthAttribute);

    // 轮廓线颜色
    m_pOutLineColorAttribute = new NDColorAttribute;
    m_pOutLineColorAttribute->setCurrentValue(QColor(10, 10, 10));
    m_pOutLineColorAttribute->setDisplayName(tr("OutLine Color: "));
    m_pOutLineColorAttribute->setName("outlineColor");
    m_pNode->addAttribute(groupName, m_pOutLineColorAttribute);

    QObject::connect(m_pOutLineWidthAttribute, &NDIntAttribute::valueChanged, this, &UICanvasPathItem::onValueChanged);
    QObject::connect(m_pOutLineColorAttribute, &NDColorAttribute::valueChanged, this, &UICanvasPathItem::onValueChanged);
}

void UICanvasPathItem::setStartPos(const QPointF& startScenePos)
{
    QPointF localPos = this->mapFromScene(startScenePos);

    // 设置起始点
    m_hasPoints = true;
    m_painterStartPos = localPos;
    m_painterPath.moveTo(localPos);

    // 设置矩形
    QRectF boundRect = m_painterPath.boundingRect();
    m_size = QSize(boundRect.width(), boundRect.height());

    this->update();
}

void UICanvasPathItem::endDrawPos(void)
{
    // 设置矩形
    QRectF boundRect = m_painterPath.boundingRect();
    m_size = QSize(boundRect.width(), boundRect.height());

    QPointF centerPos = boundRect.center();
    this->setPos(centerPos);

    m_painterPath.translate(-centerPos.x(), -centerPos.y());

    m_hadAddPointFinished = true;
    this->prepareGeometryChange();

    this->update();
}

void UICanvasPathItem::onValueChanged(void)
{
    this->update();
}
