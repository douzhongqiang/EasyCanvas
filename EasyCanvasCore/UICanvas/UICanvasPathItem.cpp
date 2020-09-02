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
    m_points.append(localPos);

    this->update();
}

void UICanvasPathItem::customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QPen pen;
    pen.setWidth(m_pOutLineWidthAttribute->getValue().toInt());
    pen.setColor(m_pOutLineColorAttribute->getValue().value<QColor>());
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
    m_pOutLineWidthAttribute->setValue(1);
    m_pOutLineWidthAttribute->setValueRange(1, 20);
    m_pOutLineWidthAttribute->setDisplayName(tr("OutLine Width: "));
    m_pOutLineWidthAttribute->setName("outlineWidth");
    m_pNode->addAttribute(groupName, m_pOutLineWidthAttribute);

    // 轮廓线颜色
    m_pOutLineColorAttribute = new NDColorAttribute;
    m_pOutLineColorAttribute->setValue(QColor(10, 10, 10));
    m_pOutLineColorAttribute->setDisplayName(tr("OutLine Color: "));
    m_pOutLineColorAttribute->setName("outlineColor");
    m_pNode->addAttribute(groupName, m_pOutLineColorAttribute);

    QObject::connect(m_pOutLineWidthAttribute, &NDIntAttribute::valueChanged, this, &UICanvasPathItem::onValueChanged);
    QObject::connect(m_pOutLineColorAttribute, &NDColorAttribute::valueChanged, this, &UICanvasPathItem::onValueChanged);
}

void UICanvasPathItem::setStartPos(const QPointF& startScenePos)
{
    QPointF localPos = this->mapFromScene(startScenePos);
    m_points.append(localPos);

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

// 获取当前属性Json
QJsonObject UICanvasPathItem::getStoreJson(int interval)
{
    QJsonObject jsonObject = UICanvasItemBase::getStoreJson(interval);

    QStringList pointStringList;
    for (auto iter = m_points.begin(); iter != m_points.end(); ++iter)
    {
        QString pointString("(%1,%2)");
        pointString = pointString.arg(iter->x()).arg(iter->y());

        pointStringList << pointString;
    }

    jsonObject.insert("PointData", pointStringList.join(";"));
    return jsonObject;
}

void UICanvasPathItem::fillJsonInfo(const QJsonObject& jsonObject)
{
    QString pointString = jsonObject.value("PointData").toString();
    QStringList pointValues = pointString.split(";");
    int count = 0;
    for (auto iter = pointValues.begin(); iter != pointValues.end(); ++iter)
    {
        QString pointValue = iter->right(iter->length() - 1);
        pointValue = pointValue.left(pointValue.length() - 1);
        QStringList values = pointValue.split(",");

        QPointF pos(values[0].toDouble(), values[1].toDouble());

        if (count == 0)
        {
            setStartPos(pos);
            count++;
            continue;
        }
        else {
            addPoint(pos);
        }
    }

    endDrawPos();
    UICanvasItemBase::fillJsonInfo(jsonObject);
}

void UICanvasPathItem::onValueChanged(void)
{
    this->update();
}
