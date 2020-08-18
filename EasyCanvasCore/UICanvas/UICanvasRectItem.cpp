#include "UICanvasRectItem.h"
#include "NDNodeBase.h"
#include "NDBoolAttribute.h"
#include "NDIntAttribute.h"
#include "NDColorAttribute.h"
#include <QPainter>

UICanvasRectItem::UICanvasRectItem(QGraphicsItem* parentItem)
    :UICanvasItemBase (parentItem)
{
    initAttribute();
}

UICanvasRectItem::~UICanvasRectItem()
{

}

void UICanvasRectItem::setSize(int width, int height)
{
    m_size.setWidth(width);
    m_size.setHeight(height);

    this->prepareGeometryChange();
}

void UICanvasRectItem::customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    // 设置画笔
    if (m_pBOutLineAttribute->getCurrentValue())
    {
        QPen pen;
        pen.setWidth(m_pOutLineWidthAttribute->getCurrentValue());
        pen.setColor(m_pOutLineColorAttribute->getCurrentValue());
        painter->setPen(pen);
    }
    else {
        painter->setPen(Qt::NoPen);
    }

    // 设置画刷
    if (m_pBFillAttribute->getCurrentValue())
    {
        painter->setBrush(QBrush(m_pFillColorAttribute->getCurrentValue()));
    }
    else {
        painter->setBrush(Qt::NoBrush);
    }

    QRectF rect = this->getCustomRect();
    painter->drawRect(rect);

    painter->restore();
}

void UICanvasRectItem::initAttribute()
{
    QString groupName = tr("Item Attribute");
    m_pNode->addAttributeGroup(groupName);

    // 是否为圆形
    m_pBRoundAttribute = new NDBoolAttribute;
    m_pBRoundAttribute->setCurrentValue(true);
    m_pBRoundAttribute->setDisplayName(tr("Is Rounded: "));
    m_pBRoundAttribute->setName("bRounded");
    m_pNode->addAttribute(groupName, m_pBRoundAttribute);

    // 添加是否填充属性
    m_pBFillAttribute = new NDBoolAttribute;
    m_pBFillAttribute->setCurrentValue(false);
    m_pBFillAttribute->setDisplayName(tr("Is Fill Color: "));
    m_pBFillAttribute->setName("bFillColor");
    m_pNode->addAttribute(groupName, m_pBFillAttribute);

    // 添加填充颜色
    m_pFillColorAttribute = new NDColorAttribute;
    m_pFillColorAttribute->setEnable(false);
    m_pFillColorAttribute->setCurrentValue(QColor(150, 150, 150));
    m_pFillColorAttribute->setDisplayName(tr("Fill Color: "));
    m_pFillColorAttribute->setName("fillColor");
    m_pNode->addAttribute(groupName, m_pFillColorAttribute);

    // 添加是否显示轮廓线
    m_pBOutLineAttribute = new NDBoolAttribute;
    m_pBOutLineAttribute->setCurrentValue(true);
    m_pBOutLineAttribute->setDisplayName(tr("Show OutLine: "));
    m_pBOutLineAttribute->setName("bOutline");
    m_pNode->addAttribute(groupName, m_pBOutLineAttribute);

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

    // 连接信号和槽
    QObject::connect(m_pBRoundAttribute, &NDBoolAttribute::valueChanged, this, &UICanvasRectItem::onRoundValueChanged);
    QObject::connect(m_pBFillAttribute, &NDBoolAttribute::valueChanged, this, &UICanvasRectItem::onValueChanged);
    QObject::connect(m_pFillColorAttribute, &NDColorAttribute::valueChanged, this, &UICanvasRectItem::onValueChanged);
    QObject::connect(m_pBOutLineAttribute, &NDBoolAttribute::valueChanged, this, &UICanvasRectItem::onValueChanged);
    QObject::connect(m_pOutLineWidthAttribute, &NDIntAttribute::valueChanged, this, &UICanvasRectItem::onValueChanged);
    QObject::connect(m_pOutLineColorAttribute, &NDColorAttribute::valueChanged, this, &UICanvasRectItem::onValueChanged);
}

void UICanvasRectItem::onValueChanged(void)
{
    this->update();
}

void UICanvasRectItem::onRoundValueChanged(bool value)
{
    this->setItemResizeable(true);
    this->setItemResizeRatio(value, 1.0);

    if (value)
    {
        int width = m_size.width();
        m_size.setHeight(width);
        this->prepareGeometryChange();
    }
}

