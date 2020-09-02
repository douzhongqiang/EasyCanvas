/**
基本组合控件-颜色控件：
作者：Douzhq
日期：2020-01-22
个人博客：不会飞的纸飞机 http://www.douzhq.cn
*/
#ifndef UICUSTOMCOLORCONTROL_H
#define UICUSTOMCOLORCONTROL_H

#include "UICustomCombineControlBase.h"
#include "UICustomWidgtes/CustomWidget.h"
#include "UICustomSingleControl/UICustomColorViewWidget.h"
#include "customcontrols_global.h"
#include <QSlider>

class CUSTOMCONTROLSSHARED_EXPORT UICustomColorControl : public UICustomCombineControlBase
{
    Q_OBJECT

public:
    UICustomColorControl(QWidget* parent = nullptr);
    ~UICustomColorControl();

    // 设置/获取当前的颜色值
    void setCurrentColor(const QColor& color, bool cmd = false);
    const QColor& getCurrentColor(void);

private:
    // 预览控件
    UICustomColorViewWidget* m_pColorViewWidget = nullptr;
    // 滑条
    QSlider* m_pSlider = nullptr;

    QColor m_tempColor;

    // 设置颜色
    void setColorByVValue(int vValue, bool cmd = false);

private slots:
    void onSliderMoved(void);
    void onSliderPressed(void);
    void onSliderReleased(void);
    void onSliderValueChanged(int value);

    void onColorViewClicked(void);

signals:
    // 颜色更改发送信号（滑杆松开后发送该信号、通过点击更改颜色）
    void colorChanged(const QColor& color, bool cmd = false);
    // 颜色更改信号（滑动滑杆就触发）
    void colorDragChanged(const QColor& color, bool cmd = false);
};

#endif
