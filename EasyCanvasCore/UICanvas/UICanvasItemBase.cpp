#include "UICanvasItemBase.h"
#include "Utils.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVector2D>
#include <QVector3D>
#include <cmath>
#include <math.h>
#include "NDNodeBase.h"
#include "NDAttributeBase.h"
#include "NDRealAttribute.h"
#include "NDIntAttribute.h"
#include "RALLBlockSignal.h"
#include "UICanvasItemManager.h"
#include "UICanvasView.h"

QImage UICanvasItemBase::m_closeIcon;
QImage UICanvasItemBase::m_resizeIcon;
QImage UICanvasItemBase::m_rotateIcon;

UICanvasItemBase::UICanvasItemBase(QGraphicsItem* parentItem)
    :QGraphicsItem(parentItem)
    ,m_cPenColor(255, 0, 0)
    ,m_cBrushColor(200, 100, 100)
{
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    initIcon();

    // 初始化基本属性
    initNodeBase();

    // 获取中心位置
    m_size = QSize(100, 100);
    QPointF centerPos = g_currentCanvasManager->getCurrentCanvasView()->getCenterPos();
    this->setPos(centerPos);
}

UICanvasItemBase::~UICanvasItemBase()
{

}

void UICanvasItemBase::setItemResizeable(bool resizeable)
{
    m_isResizeable = resizeable;
}

NDNodeBase* UICanvasItemBase::getCurrentNode(void) const
{
    return m_pNode;
}

// 拷贝另一元素到自身数据中
void UICanvasItemBase::copyFromItem(UICanvasItemBase* pItem)
{
    if (pItem == nullptr || pItem->m_pNode->getNodeType() != m_pNode->getNodeType())
        return;

    m_pXPostionAttribute->setCurrentValue(pItem->m_pXPostionAttribute->getCurrentValue());
    m_pYPostionAttribute->setCurrentValue(pItem->m_pYPostionAttribute->getCurrentValue());
    m_pZPostionAttribute->setCurrentValue(pItem->m_pZPostionAttribute->getCurrentValue());

    m_pWidthAttribute->setCurrentValue(pItem->m_pWidthAttribute->getCurrentValue());
    m_pHeightAttribute->setCurrentValue(pItem->m_pHeightAttribute->getCurrentValue());

    m_pRotateAttribute->setCurrentValue(pItem->m_pRotateAttribute->getCurrentValue());
}

void UICanvasItemBase::setItemResizeRatio(bool resizeRation, qreal rationValue)
{
    m_isRatioScale = resizeRation;
    m_ratioValue = rationValue;
}

QRectF UICanvasItemBase::boundingRect() const
{
    QRectF rectF = getCustomRect();
    if (!this->isSelected())
        return rectF;

    rectF.adjust(-m_nInterval, -m_nInterval, m_nInterval, m_nInterval);
    rectF.adjust(-m_nEllipseWidth, -m_nEllipseWidth, m_nEllipseWidth, m_nEllipseWidth);

    return rectF;
}

void UICanvasItemBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);

    // 自定义绘制
    customPaint(painter, option, widget);

    if (!this->isSelected())
        return;

    // 设置画笔
    QPen pen;
    pen.setWidth(m_nPenWidth);
    pen.setColor(m_cPenColor);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);

    QRectF itemRect = this->getCustomRect();

    // 绘制轮廓线
    QRectF outLintRect = itemRect.adjusted(-m_nInterval, -m_nInterval, m_nInterval, m_nInterval);
    painter->drawRect(outLintRect);

    painter->setPen(Qt::NoPen);
    painter->setBrush(m_cBrushColor);

    // 绘制控制点
    painter->drawEllipse(outLintRect.topRight(), m_nEllipseWidth, m_nEllipseWidth);
    if (!m_closePixmap.isNull())
        painter->drawPixmap(QRect(outLintRect.topRight().x() - m_nEllipseWidth / 2, \
                                  outLintRect.topRight().y() - m_nEllipseWidth / 2, \
                                  m_nEllipseWidth, m_nEllipseWidth), m_closePixmap);

    painter->drawEllipse(outLintRect.bottomLeft(), m_nEllipseWidth, m_nEllipseWidth);
    if (!m_rotatePixmap.isNull())
        painter->drawPixmap(QRect(outLintRect.bottomLeft().x() - m_nEllipseWidth / 2, \
                                  outLintRect.bottomLeft().y() - m_nEllipseWidth / 2, \
                                  m_nEllipseWidth, m_nEllipseWidth), m_rotatePixmap);

    if (m_isResizeable)
    {
        painter->drawEllipse(outLintRect.bottomRight(), m_nEllipseWidth, m_nEllipseWidth);
        if (!m_resizePixmap.isNull())
            painter->drawPixmap(QRect(outLintRect.bottomRight().x() - m_nEllipseWidth / 2, \
                                      outLintRect.bottomRight().y() - m_nEllipseWidth / 2, \
                                      m_nEllipseWidth, m_nEllipseWidth), m_resizePixmap);
    }
}

QPainterPath UICanvasItemBase::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());

    return path;
}

void UICanvasItemBase::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QRectF itemRect = this->getCustomRect();
    QRectF outLintRect = itemRect.adjusted(-m_nInterval, -m_nInterval, m_nInterval, m_nInterval);

    // 获取当前模式
    QPointF pos = event->pos();
    QPointF scenePos = event->scenePos();
    if (itemRect.contains(pos))
        m_itemOper = t_move;
    else if (g_utilTool->getDistance(pos, outLintRect.topRight()) <= m_nEllipseWidth)
    {
        g_currentCanvasManager->deleteCanvasItem(m_pNode);
        return;
    }
    else if (g_utilTool->getDistance(pos, outLintRect.bottomLeft()) <= m_nEllipseWidth)
        m_itemOper = t_rotate;
    else if (g_utilTool->getDistance(pos, outLintRect.bottomRight()) <= m_nEllipseWidth)
        m_itemOper = t_resize;

    // 保存当前的一些信息
    m_pos = pos;
    m_pressedPos = scenePos;
    m_startPos = this->pos();
    return QGraphicsItem::mousePressEvent(event);
}

void UICanvasItemBase::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // 获取场景坐标和本地坐标
    QPointF scenePos = event->scenePos();
    QPointF pos = event->pos();

    if (m_itemOper == t_move)
    {
        // 处理移动
        mouseMoveMoveOperator(scenePos, pos);
    }
    else if (m_itemOper == t_resize)
    {
        // 处理更改大小
        mouseMoveResizeOperator(scenePos, pos);
    }
    else if (m_itemOper == t_rotate)
    {
        // 处理旋转
        mouseMoveRotateOperator(scenePos, pos);
    }

    return QGraphicsItem::mouseMoveEvent(event);
}

void UICanvasItemBase::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_itemOper = t_none;
    return QGraphicsItem::mouseReleaseEvent(event);
}

QVariant UICanvasItemBase::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedChange)
        prepareGeometryChange();
    else if (change == QGraphicsItem::ItemPositionHasChanged)
    {
        updateAttribute();
    }

    return QGraphicsItem::itemChange(change, value);
}

void UICanvasItemBase::mouseMoveMoveOperator(const QPointF& scenePos, const QPointF& loacalPos)
{
    qreal xInterval = scenePos.x() - m_pressedPos.x();
    qreal yInterval = scenePos.y() - m_pressedPos.y();

    this->setPos(m_startPos + QPointF(xInterval, yInterval));
    this->update();
}

void UICanvasItemBase::mouseMoveResizeOperator(const QPointF& scenePos, const QPointF& loacalPos)
{
    if (!m_isResizeable)
        return;

    qreal ratio = m_ratioValue;
    qreal itemWidth = abs(loacalPos.x()) * 2 - m_nInterval - m_nEllipseWidth;
    qreal itemHeight = abs(loacalPos.y()) * 2 - m_nInterval - m_nEllipseWidth;
    if (m_isRatioScale)
        itemHeight = itemWidth * 1.0 / ratio;

    // 设置图片的最小大小为10
    if (itemWidth < 10 || itemHeight < 10)
        return;

    m_size = QSize(itemWidth, itemHeight);
    m_pWidthAttribute->setCurrentValue(m_size.width());
    m_pHeightAttribute->setCurrentValue(m_size.height());

    this->update();
}

void UICanvasItemBase::mouseMoveRotateOperator(const QPointF& scenePos, const QPointF& loacalPos)
{
    // 获取并设置为单位向量
    QVector2D startVec(m_pos.x() - 0, m_pos.y() - 0);
    startVec.normalize();
    QVector2D endVec(loacalPos.x() - 0, loacalPos.y() - 0);
    endVec.normalize();

    // 单位向量点乘，计算角度
    qreal dotValue = QVector2D::dotProduct(startVec, endVec);
    if (dotValue > 1.0)
        dotValue = 1.0;
    else if (dotValue < -1.0)
        dotValue = -1.0;

    dotValue = qAcos(dotValue);
    if (isnan(dotValue))
        dotValue = 0.0;

    // 获取角度
    qreal angle = dotValue * 1.0 / (PI / 180);

    // 向量叉乘获取方向
    QVector3D crossValue = QVector3D::crossProduct(QVector3D(startVec, 1.0),QVector3D(endVec, 1.0));
    if (crossValue.z() < 0)
        angle = -angle;
    m_rotate += angle;

    // 设置角度在0~360之间
    if (m_rotate >= 360)
        m_rotate -= 360;
    else if (m_rotate < 0)
        m_rotate += 360;

    // 设置变化矩阵
    QTransform transform;
    transform.rotate(m_rotate);
    this->setTransform(transform);
    m_pRotateAttribute->setCurrentValue(m_rotate);

    this->update();
}

void UICanvasItemBase::customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

QRectF UICanvasItemBase::getCustomRect(void) const
{
    QPointF centerPos(0, 0);
    return QRectF(centerPos.x() - m_size.width() / 2, centerPos.y() - m_size.height() / 2, \
                  m_size.width(), m_size.height());
}

void UICanvasItemBase::initIcon(void)
{
    if (m_closeIcon.isNull())
        m_closeIcon.load("./Images/close.png");
    if (m_resizeIcon.isNull())
        m_resizeIcon.load("./Images/resize.png");
    if (m_rotateIcon.isNull())
        m_rotateIcon.load("./Images/rotate.png");

    m_closePixmap = QPixmap::fromImage(m_closeIcon);
    m_resizePixmap = QPixmap::fromImage(m_resizeIcon);
    m_rotatePixmap = QPixmap::fromImage(m_rotateIcon);
}

void UICanvasItemBase::initNodeBase(void)
{
    // 创建节点
    m_pNode = new NDNodeBase(this);

    // 添加属性组
    QString attributeGroupString = tr("Base Attribute");
    NDAttributeGroup* pGroup = m_pNode->addAttributeGroup("BaseAttr", attributeGroupString);

    // X坐标
    m_pXPostionAttribute = new NDRealAttribute;
    m_pXPostionAttribute->setDisplayName(tr("X Postion: "));
    m_pXPostionAttribute->setName("xPt");
    m_pXPostionAttribute->setValueRange(0, 5000);
    m_pNode->addAttribute(pGroup, m_pXPostionAttribute);
    // Y坐标
    m_pYPostionAttribute = new NDRealAttribute;
    m_pYPostionAttribute->setDisplayName(tr("Y Postion: "));
    m_pYPostionAttribute->setName("yPt");
    m_pYPostionAttribute->setValueRange(0, 5000);
    m_pNode->addAttribute(pGroup, m_pYPostionAttribute);
    // Z坐标
    m_pZPostionAttribute = new NDRealAttribute;
    m_pZPostionAttribute->setDisplayName(tr("Z Postion: "));
    m_pZPostionAttribute->setName("zPt");
    m_pZPostionAttribute->setValueRange(0, 10);
    m_pNode->addAttribute(pGroup, m_pZPostionAttribute);

    // 宽度
    m_pWidthAttribute = new NDIntAttribute;
    m_pWidthAttribute->setDisplayName(tr("width: "));
    m_pWidthAttribute->setName("width");
    m_pWidthAttribute->setValueRange(10, 5000);
    m_pNode->addAttribute(pGroup, m_pWidthAttribute);
    // 高度
    m_pHeightAttribute = new NDIntAttribute;
    m_pHeightAttribute->setDisplayName(tr("height: "));
    m_pHeightAttribute->setName("height");
    m_pHeightAttribute->setValueRange(10, 5000);
    m_pNode->addAttribute(pGroup, m_pHeightAttribute);

    // 旋转
    m_pRotateAttribute = new NDRealAttribute;
    m_pRotateAttribute->setDisplayName(tr("Rotate Angle: "));
    m_pRotateAttribute->setName("rotate");
    m_pRotateAttribute->setValueRange(0, 360);
    m_pRotateAttribute->setCurrentValue(0);
    m_pNode->addAttribute(pGroup, m_pRotateAttribute);

    // 连接信号和槽
    QObject::connect(m_pXPostionAttribute, &NDRealAttribute::valueChanged, this, &UICanvasItemBase::onXPostionAttributeValueChanged);
    QObject::connect(m_pYPostionAttribute, &NDRealAttribute::valueChanged, this, &UICanvasItemBase::onYPostionAttributeValueChanged);
    QObject::connect(m_pZPostionAttribute, &NDRealAttribute::valueChanged, this, &UICanvasItemBase::onZPostionAttributeValueChanged);
    QObject::connect(m_pWidthAttribute, &NDIntAttribute::valueChanged, this, &UICanvasItemBase::onWidthAttributeValueChanged);
    QObject::connect(m_pHeightAttribute, &NDIntAttribute::valueChanged, this, &UICanvasItemBase::onHeightAttributeValueChanged);
    QObject::connect(m_pRotateAttribute, &NDRealAttribute::valueChanged, this, &UICanvasItemBase::onRotateAttributeValueChanged);
}

void UICanvasItemBase::updateAttribute(void)
{
    QPointF pos = this->pos();

    m_pXPostionAttribute->setCurrentValue(pos.x());
    m_pYPostionAttribute->setCurrentValue(pos.y());
    m_pZPostionAttribute->setCurrentValue(this->zValue());

    m_pWidthAttribute->setCurrentValue(m_size.width());
    m_pHeightAttribute->setCurrentValue(m_size.height());

    m_pRotateAttribute->setCurrentValue(m_rotate);
}

void UICanvasItemBase::onXPostionAttributeValueChanged(qreal value)
{
    QPointF pos = this->pos();
    pos.setX(value);

    this->setPos(pos);
}

void UICanvasItemBase::onYPostionAttributeValueChanged(qreal value)
{
    QPointF pos = this->pos();
    pos.setY(value);

    this->setPos(pos);
}

void UICanvasItemBase::onZPostionAttributeValueChanged(qreal value)
{
    this->setZValue(value);
}

void UICanvasItemBase::onWidthAttributeValueChanged(int value)
{
    if (!m_isResizeable)
    {
        RALLBlockSingal wdithBlockSingal(m_pWidthAttribute);
        m_pWidthAttribute->setCurrentValue(m_size.width());
        return;
    }

    qreal ratio = m_ratioValue;
    qreal itemWidth = value;
    qreal itemHeight = m_size.height();
    if (m_isRatioScale)
        itemHeight = itemWidth * 1.0 / ratio;

    // 设置图片的最小大小为10
    if (itemWidth < 10 || itemHeight < 10)
    {
        RALLBlockSingal wdithBlockSingal(m_pWidthAttribute);
        m_pWidthAttribute->setCurrentValue(m_size.width());
        return;
    }

    m_size.setWidth(itemWidth);
    m_size.setHeight(itemHeight);

    QObject::disconnect(m_pHeightAttribute, &NDIntAttribute::valueChanged, this, &UICanvasItemBase::onHeightAttributeValueChanged);
    m_pHeightAttribute->setCurrentValue(itemHeight);
    QObject::connect(m_pHeightAttribute, &NDIntAttribute::valueChanged, this, &UICanvasItemBase::onHeightAttributeValueChanged);

    this->update();
    this->prepareGeometryChange();
}

void UICanvasItemBase::onHeightAttributeValueChanged(int value)
{
    if (!m_isResizeable)
    {
        RALLBlockSingal wdithBlockSingal(m_pHeightAttribute);
        m_pHeightAttribute->setCurrentValue(m_size.height());
        return;
    }

    qreal ratio = m_ratioValue;
    qreal itemWidth = m_size.width();
    qreal itemHeight = value;
    if (m_isRatioScale)
        itemWidth = itemHeight * 1.0 * ratio;

    // 设置图片的最小大小为10
    if (itemWidth < 10 || itemHeight < 10)
    {
        RALLBlockSingal wdithBlockSingal(m_pHeightAttribute);
        m_pHeightAttribute->setCurrentValue(m_size.height());
        return;
    }

    m_size.setWidth(itemWidth);
    m_size.setHeight(itemHeight);

    QObject::disconnect(m_pWidthAttribute, &NDIntAttribute::valueChanged, this, &UICanvasItemBase::onWidthAttributeValueChanged);
    m_pWidthAttribute->setCurrentValue(itemWidth);
    QObject::connect(m_pWidthAttribute, &NDIntAttribute::valueChanged, this, &UICanvasItemBase::onWidthAttributeValueChanged);

    this->update();
    this->prepareGeometryChange();
}

void UICanvasItemBase::onRotateAttributeValueChanged(qreal value)
{
    m_rotate = value;

    QTransform transform;
    transform.rotate(m_rotate);
    this->setTransform(transform);

    this->update();
}
