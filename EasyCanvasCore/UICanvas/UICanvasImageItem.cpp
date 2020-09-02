#include "UICanvasImageItem.h"
#include "NDStringAttribute.h"
#include "NDNodeBase.h"
#include <QPainter>
#include <QFileDialog>
#include <functional>

UICanvasImageItem::UICanvasImageItem(QGraphicsItem* parentItem)
    :UICanvasItemBase(parentItem)
{
    initAttribute();
    setImage("./t3.jpg");
}

UICanvasImageItem::~UICanvasImageItem()
{

}

void UICanvasImageItem::customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QRectF customRect = this->getCustomRect();
    QRect pixRect(customRect.x(), customRect.y(), customRect.width(), customRect.height());
    painter->drawPixmap(pixRect, m_pixmap);

    painter->restore();
}

void UICanvasImageItem::setImage(const QString& filePath)
{
    QImage image(filePath);

    m_pixmap = QPixmap::fromImage(image);
    if (!m_pixmap.isNull())
    {
        // 设置 size
        m_size.setWidth(m_pixmap.width());
        m_size.setHeight(m_pixmap.height());

        // 设置改变大小属性
        this->setItemResizeable(true);
        this->setItemResizeRatio(true, m_size.width() * 1.0 / m_size.height());

        // 设置属性
        QObject::disconnect(m_pFilePathAttribute, &NDStringAttribute::valueChanged, this, &UICanvasImageItem::onAttributeValueChanged);
        m_pFilePathAttribute->setValue(filePath);
        QObject::connect(m_pFilePathAttribute, &NDStringAttribute::valueChanged, this, &UICanvasImageItem::onAttributeValueChanged);
    }

    this->update();
}

void UICanvasImageItem::initAttribute(void)
{
    // 添加属性组
    QString attributeGroupName = tr("Image Attribute");
    m_pNode->addAttributeGroup(attributeGroupName);

    // 创建属性
    m_pFilePathAttribute = new NDStringAttribute;
    m_pFilePathAttribute->setDisplayName(tr("Image Path: "));
    m_pFilePathAttribute->setName("imagePath");
    m_pFilePathAttribute->setShowButton(true);
    m_pFilePathAttribute->setButtonString(tr("Browse"));
    m_pFilePathAttribute->setButtonFunction(std::bind(&UICanvasImageItem::showFileBrowseDialo, \
                                                      this, std::placeholders::_1));
    m_pNode->addAttribute(attributeGroupName, m_pFilePathAttribute);

    // 连接信号和槽函数
    QObject::connect(m_pFilePathAttribute, &NDStringAttribute::valueChanged, \
                     this, &UICanvasImageItem::onAttributeValueChanged);
}

bool UICanvasImageItem::showFileBrowseDialo(QString& str)
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open Image File"), "./", \
                                 tr("Image Files (*.bmp *.png *.jpg)"));
    if (fileName.isEmpty())
        return false;

    str = fileName;
    return true;
}

void UICanvasImageItem::onAttributeValueChanged(const QVariant& value)
{
    this->setImage(value.toString());
}
