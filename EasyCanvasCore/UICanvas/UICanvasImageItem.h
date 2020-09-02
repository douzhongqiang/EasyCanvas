#ifndef UICANVASIMAGEITEM_H
#define UICANVASIMAGEITEM_H

#include "UICanvasItemBase.h"
#include <QPixmap>

class NDStringAttribute;
class UICanvasImageItem : public UICanvasItemBase
{
    Q_OBJECT

public:
    UICanvasImageItem(QGraphicsItem* parentItem = nullptr);
    ~UICanvasImageItem();

    // 设置图像
    void setImage(const QString& filePath);

protected:
    // 自定义元素绘制
    virtual void customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QPixmap m_pixmap;

    // 初始化属性
    void initAttribute(void);
    // 图片文件目录
    NDStringAttribute* m_pFilePathAttribute = nullptr;
    // 设置函数
    bool showFileBrowseDialo(QString& str);

private slots:
    void onAttributeValueChanged(const QVariant& value);
};

#endif
