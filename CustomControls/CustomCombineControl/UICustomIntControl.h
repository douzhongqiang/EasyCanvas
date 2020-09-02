/**
基本组合控件-整形值控件：
作者：Douzhq
日期：2020-01-22
个人博客：不会飞的纸飞机 www.douzhq.cn
*/
#ifndef UICUSTOMINTCONTROL_H
#define UICUSTOMINTCONTROL_H

#include "UICustomCombineControlBase.h"
#include <QWidget>
#include <QSpinBox>
#include <QSlider>
#include "customcontrols_global.h"

class UICustomIntSpinBox;
class CUSTOMCONTROLSSHARED_EXPORT UICustomIntControl : public UICustomCombineControlBase
{
    Q_OBJECT

public:
    UICustomIntControl(QWidget* parent = nullptr);
    ~UICustomIntControl();

    // 设置/获取当前的值
    void setCurrentValue(int value);
    int getCurrentValue(void);

    // 设置范围
    void setRangeValue(int minValue, int maxValue);

private:
    UICustomIntSpinBox* m_pIntValue = nullptr;
    QSlider* m_pSlider = nullptr;

    int m_nMaxValue = 100;
    int m_nMinValue = 0;
    int m_tempValue = 0;

    int getValuesBySlider(void);

private slots:
    void onSliderPressed(void);
    void onSliderMoved(void);
    void onSliderReleased(void);
    void onSliderValueChanged(int);

    void onIntValueChanged(void);

signals:
    void valueChanged(int value, bool cmd = false);
};

#endif
