#ifndef UIATTRCOLORCONTROL_H
#define UIATTRCOLORCONTROL_H

#include "CustomCombineControl/UICustomColorControl.h"
#include "NDAttributeBase.h"
#include "NDColorAttribute.h"

class UIAttrColorControl : public UICustomColorControl
{
    Q_OBJECT

public:
    UIAttrColorControl(NDAttributeBase* attribute = nullptr, QWidget* parent = nullptr);
    ~UIAttrColorControl();

    // 设置属性
    void setAttribute(NDAttributeBase* attribute);

private:
    NDColorAttribute* m_attribute = nullptr;

private slots:
    // 属性值发生改变同步控件
    void onColorValueChanged(const QVariant& value);
    // 控件值发生改变同步属性
    void onControlValuedChanged(const QColor& color, bool cmd = false);
};

#endif
