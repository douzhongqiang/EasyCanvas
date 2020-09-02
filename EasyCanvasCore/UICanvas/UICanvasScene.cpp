#include "UICanvasScene.h"
#include "NDNodeBase.h"
#include "NDIntAttribute.h"
#include "NDColorAttribute.h"
#include "UICanvasItemManager.h"
#include <QPainter>

UICanvasScene::UICanvasScene(QObject* parent)
    :QGraphicsScene(parent)
{
    initNodeInfo();

    int width = m_pWidthAttribute->getValue().toInt();
    int height = m_pHeightAttribute->getValue().toInt();
    this->setSceneRect(QRectF(0, 0, width, height));
}

UICanvasScene::~UICanvasScene()
{

}

void UICanvasScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect, QColor(60, 60, 60));
    QRectF sceneRect = this->sceneRect();
    painter->fillRect(sceneRect, QBrush(m_pBackgroundColorAttribute->getValue().value<QColor>()));
    return QGraphicsScene::drawBackground(painter, rect);
}

void UICanvasScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    return QGraphicsScene::drawForeground(painter, rect);
}

void UICanvasScene::initNodeInfo(void)
{
    m_pNode = new NDNodeBase(this);
    QString groupString = tr("Canvas Attribute");
    m_pNode->addAttributeGroup(groupString);
    m_pNode->setNodeName("canvas");
    m_pNode->setNodeType((int)UICanvasItemManager::t_CanvasItem);

    // 宽度
    m_pWidthAttribute = new NDIntAttribute;
    m_pWidthAttribute->setDisplayName(tr("width: "));
    m_pWidthAttribute->setName("width");
    m_pWidthAttribute->setValueRange(10, 5000);
    m_pWidthAttribute->setValue(800);
    m_pNode->addAttribute(groupString, m_pWidthAttribute);
    // 高度
    m_pHeightAttribute = new NDIntAttribute;
    m_pHeightAttribute->setDisplayName(tr("height: "));
    m_pHeightAttribute->setName("height");
    m_pHeightAttribute->setValueRange(10, 5000);
    m_pHeightAttribute->setValue(500);
    m_pNode->addAttribute(groupString, m_pHeightAttribute);
    // 颜色
    m_pBackgroundColorAttribute = new NDColorAttribute;
    m_pBackgroundColorAttribute->setValue(QColor(255, 255, 255));
    m_pBackgroundColorAttribute->setDisplayName(tr("Canvas Color: "));
    m_pBackgroundColorAttribute->setName("canvasColor");
    m_pNode->addAttribute(groupString, m_pBackgroundColorAttribute);

    QObject::connect(m_pWidthAttribute, &NDIntAttribute::valueChanged, this, &UICanvasScene::onWidthAttributeValueChanged);
    QObject::connect(m_pHeightAttribute, &NDIntAttribute::valueChanged, this, &UICanvasScene::onHeightAttributeValueChanged);
    QObject::connect(m_pBackgroundColorAttribute, &NDColorAttribute::valueChanged, this, &UICanvasScene::onColorAttributeValueChanged);
}

NDNodeBase* UICanvasScene::getCurrentNode(void)
{
    return m_pNode;
}

void UICanvasScene::resetNodeInfo(void)
{
    m_pWidthAttribute->setValue(800);
    m_pHeightAttribute->setValue(500);
    m_pBackgroundColorAttribute->setValue(QColor(255, 255, 255));
}

void UICanvasScene::onWidthAttributeValueChanged(const QVariant& value)
{
    QRectF rect = this->sceneRect();
    rect.setWidth(value.toInt());
    this->setSceneRect(rect);
}

void UICanvasScene::onHeightAttributeValueChanged(const QVariant& value)
{
    QRectF rect = this->sceneRect();
    rect.setHeight(value.toInt());
    this->setSceneRect(rect);
}

void UICanvasScene::onColorAttributeValueChanged(const QVariant& color)
{
    this->update();
}
