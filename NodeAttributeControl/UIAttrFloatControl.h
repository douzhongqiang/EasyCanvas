#ifndef UIATTRFLOATCONTROL_H
#define UIATTRFLOATCONTROL_H

#include "CustomCombineControl/UICustomDoubleControl.h"
#include "NDNodeBase.h"
#include "NDRealAttribute.h"

class UIAttrFloatControl : public UICustomDoubleControl
{
    Q_OBJECT

public:
    UIAttrFloatControl(NDAttributeBase* attribute = nullptr, QWidget* parent = nullptr);
    ~UIAttrFloatControl();

    // 设置属性
    void setAttribute(NDAttributeBase* attribute);

private:
    NDRealAttribute* m_attribute = nullptr;

private slots:
    void onValueChanged(const QVariant& value);
    void onControlValueChanged(qreal value, bool cmd = false);
};

#endif
