/**
基本组合控件-浮点值控件：
作者：Douzhq
日期：2020-01-22
个人博客：不会飞的纸飞机 www.douzhq.cn
*/
#ifndef UICUSTOMDOUBLECONTROL_H
#define UICUSTOMDOUBLECONTROL_H

#include "UICustomWidgtes/CustomWidget.h"
#include "UICustomCombineControlBase.h"
#include "customcontrols_global.h"
#include <QWidget>
#include <QDoubleSpinBox>
#include <QSlider>

class UICustomDoubleSpinBox;
class CUSTOMCONTROLSSHARED_EXPORT UICustomDoubleControl : public UICustomCombineControlBase
{
    Q_OBJECT

public:
    UICustomDoubleControl(QWidget* parent = nullptr);
    ~UICustomDoubleControl();

    // 设置/获取当前值
    void setCurrentValue(qreal value);
    qreal getCurrentValue(void);

    // 设置范围
    void setRangeValue(qreal minValue, qreal maxValue);

private:
    UICustomDoubleSpinBox* m_pDoubleSpinBox = nullptr;
    QSlider* m_pSlider = nullptr;

    qreal m_beforeValue = 0;
    qreal m_qMaxValue = 0;
    qreal m_qMinValue = 1;

    qreal getValuesBySlider(void);

private slots:
    void onSliderPressed(void);
    void onSliderMoved(void);
    void onSliderReleased(void);
    void onSliderValueChanged(int value);

    void onDoubleValueChanged(void);

signals:
    void valueChanged(qreal value, bool cmd = false);
};

#endif
