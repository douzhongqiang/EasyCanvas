#include "UICanvasTextItem.h"
#include "NDNodeBase.h"
#include "NDBoolAttribute.h"
#include "NDIntAttribute.h"
#include "NDColorAttribute.h"
#include "NDStringAttribute.h"
#include <QPainter>
#include <QApplication>

UICanvasTextItem::UICanvasTextItem(QGraphicsItem* parentItem)
    :UICanvasItemBase(parentItem)
{
    m_font = qApp->font();
    m_font.setPixelSize(25);

    this->setItemResizeable(false);
    initAttribute();

    setCurrentText("EasyCanvas");
}

UICanvasTextItem::~UICanvasTextItem()
{

}

void UICanvasTextItem::setCurrentText(const QString& text)
{
    m_cText = text;

    QObject::disconnect(m_pTextValueAttribute, &NDStringAttribute::valueChanged, this, &UICanvasTextItem::onTextValueChanged);
    m_pTextValueAttribute->setValue(m_cText);
    QObject::connect(m_pTextValueAttribute, &NDStringAttribute::valueChanged, this, &UICanvasTextItem::onTextValueChanged);

    QFontMetrics fm(m_font);
    QRect rect = fm.boundingRect(m_cText);

    m_size = QSize(rect.width(), rect.height());
    this->prepareGeometryChange();

    this->update();
}

void UICanvasTextItem::setCurrentFont(const QFont& font)
{
    m_font = font;

    QFontMetrics fm(m_font);
    QRect rect = fm.boundingRect(m_cText);

    m_size = QSize(rect.width(), rect.height());
    this->prepareGeometryChange();

    this->update();
}

void UICanvasTextItem::customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QRectF textRect = this->getCustomRect();
    QPainterPath path;
    path.addText(textRect.bottomLeft(), m_font, m_cText);

    // 添加轮廓
    QPen pen;
    pen.setWidth(m_pOutLineWidthAttribute->getValue().toInt());
    pen.setColor(m_pOutLineColorAttribute->getValue().value<QColor>());
    if (m_pBOutLineAttribute->getValue().toBool())
        painter->strokePath(path, pen);

    // 绘制
    pen.setWidth(m_pPenSizeAttribute->getValue().toInt());
    pen.setColor(m_pTtextColorAttribute->getValue().value<QColor>());
    painter->setPen(pen);
    if (!m_pBFillColorAttribute->getValue().toBool())
        painter->setBrush(Qt::NoBrush);
    else
        painter->setBrush(m_pFillColorAttribute->getValue().value<QColor>());
    painter->drawPath(path);

    painter->restore();
}

void UICanvasTextItem::initAttribute(void)
{
    QString groupName = tr("Text Attribute");
    m_pNode->addAttributeGroup(groupName);

    // 本文内容
    m_pTextValueAttribute = new NDStringAttribute;
    m_pTextValueAttribute->setValue(m_cText);
    m_pTextValueAttribute->setDisplayName(tr("Text: "));
    m_pTextValueAttribute->setName("text");
    m_pTextValueAttribute->setShowButton(false);
    m_pNode->addAttribute(groupName, m_pTextValueAttribute);

    // 字体大小
    m_pFontSizeAttribute = new NDIntAttribute;
    m_pFontSizeAttribute->setValue(m_font.pixelSize());
    m_pFontSizeAttribute->setValueRange(1, 200);
    m_pFontSizeAttribute->setDisplayName(tr("Font Size: "));
    m_pFontSizeAttribute->setName("fontSize");
    m_pNode->addAttribute(groupName, m_pFontSizeAttribute);

    // 画笔大小
    m_pPenSizeAttribute = new NDIntAttribute;
    m_pPenSizeAttribute->setValue(1);
    m_pPenSizeAttribute->setValueRange(1, 10);
    m_pPenSizeAttribute->setDisplayName(tr("Pen Size: "));
    m_pPenSizeAttribute->setName("penSize");
    m_pNode->addAttribute(groupName, m_pPenSizeAttribute);

    // 文字颜色
    m_pTtextColorAttribute = new NDColorAttribute;
    m_pTtextColorAttribute->setValue(QColor(10, 10, 10));
    m_pTtextColorAttribute->setDisplayName(tr("Text Color: "));
    m_pTtextColorAttribute->setName("textColor");
    m_pNode->addAttribute(groupName, m_pTtextColorAttribute);

    // 是否填充颜色
    m_pBFillColorAttribute = new NDBoolAttribute;
    m_pBFillColorAttribute->setValue(true);
    m_pBFillColorAttribute->setDisplayName(tr("Is Fill Color: "));
    m_pBFillColorAttribute->setName("bFillColorOutline");
    m_pNode->addAttribute(groupName, m_pBFillColorAttribute);

    // 文字填充颜色
    m_pFillColorAttribute = new NDColorAttribute;
    m_pFillColorAttribute->setValue(QColor(10, 10, 10));
    m_pFillColorAttribute->setDisplayName(tr("Fill Color: "));
    m_pFillColorAttribute->setName("fillColor");
    m_pNode->addAttribute(groupName, m_pFillColorAttribute);

    QString outLineGroupName = tr("OutLine Attribute");
    m_pNode->addAttributeGroup(outLineGroupName);
    // 是否显示轮廓线
    m_pBOutLineAttribute = new NDBoolAttribute;
    m_pBOutLineAttribute->setValue(false);
    m_pBOutLineAttribute->setDisplayName(tr("Is Show Outline: "));
    m_pBOutLineAttribute->setName("bShowOutline");
    m_pNode->addAttribute(outLineGroupName, m_pBOutLineAttribute);

    // 添加填充颜色
    m_pOutLineColorAttribute = new NDColorAttribute;
    m_pOutLineColorAttribute->setEnable(false);
    m_pOutLineColorAttribute->setValue(QColor(200, 200, 200));
    m_pOutLineColorAttribute->setDisplayName(tr("Outline Color: "));
    m_pOutLineColorAttribute->setName("outlineColor");
    m_pNode->addAttribute(outLineGroupName, m_pOutLineColorAttribute);

    // 轮廓线宽度
    m_pOutLineWidthAttribute = new NDIntAttribute;
    m_pOutLineWidthAttribute->setValue(1);
    m_pOutLineWidthAttribute->setValueRange(1, 20);
    m_pOutLineWidthAttribute->setDisplayName(tr("OutLine Width: "));
    m_pOutLineWidthAttribute->setName("outlineWidth");
    m_pNode->addAttribute(outLineGroupName, m_pOutLineWidthAttribute);

    // 连接信号和槽
    QObject::connect(m_pBOutLineAttribute, &NDBoolAttribute::valueChanged, this, &UICanvasTextItem::onValueChanged);
    QObject::connect(m_pOutLineWidthAttribute, &NDIntAttribute::valueChanged, this, &UICanvasTextItem::onValueChanged);
    QObject::connect(m_pOutLineColorAttribute, &NDColorAttribute::valueChanged, this, &UICanvasTextItem::onValueChanged);

    QObject::connect(m_pFontSizeAttribute, &NDIntAttribute::valueChanged, this, &UICanvasTextItem::onFontSizeValueChanged);
    QObject::connect(m_pPenSizeAttribute, &NDIntAttribute::valueChanged, this, &UICanvasTextItem::onValueChanged);
    QObject::connect(m_pTtextColorAttribute, &NDColorAttribute::valueChanged, this, &UICanvasTextItem::onValueChanged);
    QObject::connect(m_pTextValueAttribute, &NDStringAttribute::valueChanged, this, &UICanvasTextItem::onTextValueChanged);
    QObject::connect(m_pBFillColorAttribute, &NDBoolAttribute::valueChanged, this, &UICanvasTextItem::onValueChanged);
    QObject::connect(m_pFillColorAttribute, &NDColorAttribute::valueChanged, this, &UICanvasTextItem::onValueChanged);
}

void UICanvasTextItem::onValueChanged(void)
{
    this->update();
}

void UICanvasTextItem::onTextValueChanged(const QVariant& text)
{
    setCurrentText(text.toString());
    this->update();
}

void UICanvasTextItem::onFontSizeValueChanged(const QVariant& fontSize)
{
    m_font.setPixelSize(fontSize.toInt());
    setCurrentFont(m_font);
    this->update();
}
