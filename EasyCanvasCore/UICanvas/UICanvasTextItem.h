#ifndef UICANVASTEXTITEM_H
#define UICANVASTEXTITEM_H

#include <QGraphicsItem>
#include <QFont>
#include "UICanvasItemBase.h"
#include "NDNodeManager.h"
#include "NDNodeBase.h"

class NDBoolAttribute;
class NDIntAttribute;
class NDColorAttribute;
class NDStringAttribute;
class UICanvasTextItem : public UICanvasItemBase
{
    Q_OBJECT

public:
    UICanvasTextItem(QGraphicsItem* parentItem = nullptr);
    ~UICanvasTextItem();

    // 设置文本
    void setCurrentText(const QString& text);
    // 设置字体
    void setCurrentFont(const QFont& font);

protected:
    // 自定义元素绘制
    void customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QString m_cText;
    QFont m_font;

    void initAttribute(void);

    NDBoolAttribute* m_pBOutLineAttribute = nullptr;                // 是否显示轮廓线
    NDIntAttribute* m_pOutLineWidthAttribute = nullptr;             // 轮廓线宽度
    NDColorAttribute* m_pOutLineColorAttribute = nullptr;           // 轮廓线颜色

    NDIntAttribute* m_pFontSizeAttribute = nullptr;                 // 字体大小
    NDIntAttribute* m_pPenSizeAttribute = nullptr;                  // 画笔大小
    NDColorAttribute* m_pTtextColorAttribute = nullptr;               // 文字颜色
    NDBoolAttribute* m_pBFillColorAttribute = nullptr;              // 是否填充颜色
    NDColorAttribute* m_pFillColorAttribute = nullptr;              // 填充颜色

    NDStringAttribute* m_pTextValueAttribute = nullptr;             // 文本内容

private slots:
    void onValueChanged(void);
    void onTextValueChanged(const QVariant& text);
    void onFontSizeValueChanged(const QVariant& fontSize);
};

#endif
