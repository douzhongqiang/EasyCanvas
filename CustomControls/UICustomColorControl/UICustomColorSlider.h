#ifndef UICUSTOMCOLORSLIDER_H
#define UICUSTOMCOLORSLIDER_H

#include "UICustomWidgtes/CustomWidget.h"
#include "UICustomColorSliderItem.h"
#include "CustomCombineControl/UICustomCombineControlBase.h"
#include "UICustomSingleControl/UICustomColorViewWidget.h"
#include "UICustomSingleControl/UICustomIntSpinBox.h"

class UICustomColorSlider : public UICustomCombineControlBase
{
    Q_OBJECT

public:
    UICustomColorSlider(QWidget *parent = nullptr);
    ~UICustomColorSlider();

    // 设置 最小值、最大值
    void setValueRange(int minValue, int maxValue);

    // 设置开始颜色
    void setStartColor(const QColor& startColor);
    // 设置结束颜色
    void setEndColor(const QColor& endColor);

    // 设置当前值
    void setCurrentValue(int value);
    // 获取当前值
    int getCurrentValue(void);

    // 设置是否为H
    void setHVisible(bool isVisible);

private:
    UICustomColorSliderItem* m_pColorSliderItem = nullptr;
    UICustomIntSpinBox* m_pValueSpixBox = nullptr;

    int m_nMinValue = 0;
    int m_nMaxValue = 100;

private slots:
    void onChangedValue(int value);

signals:
    void valueChanged(int value);
};

#endif
