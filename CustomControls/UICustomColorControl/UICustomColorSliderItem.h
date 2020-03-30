#ifndef UICUSTOMCOLORSLIDERITEM_H
#define UICUSTOMCOLORSLIDERITEM_H

#include <QWidget>
#include "UICustomWidgtes/CustomWidget.h"
class QColor;
class UICustomColorSliderItem : public CustomWidget
{
    Q_OBJECT

public:
    UICustomColorSliderItem(QWidget* parent = nullptr);
    ~UICustomColorSliderItem();

    // 设置开始颜色
    void setStartColor(const QColor& startColor);
    // 设置结束颜色
    void setEndColor(const QColor& endColor);

    // 设置当前选中的值
    void setCurrentValue(int value);
    // 获取当前选中值
    int getCurrentValue(void);

    // 设置是否为H
    void setHVisible(bool isVisible);

protected:
    void customPaint(QPainter* painter) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    QSize sizeHint() const override;

private:
    int m_nMargin = 1;
    int m_nSliderWidth = 12;     // 滑块宽度
    int m_nSliderHeight = 10;    // 滑块高度
    int m_nInterVal = 4;

    int m_nCurrentValue = 0;    // 当前选中数值
    int m_nMinValue = 0;        // 最小值
    int m_nMaxValue = 100;      // 最大值

    QColor m_cStartColor;       // 开始颜色
    QColor m_cEndColor;         // 结束颜色

    QVector<QColor> m_colorVec;
    bool m_isHValueSlider = false;

    bool m_isSelected = false;
    // 根据点更新显示
    void selectedPoint(const QPoint& pos);

signals:
    void changedValue(int value);
};
#endif
