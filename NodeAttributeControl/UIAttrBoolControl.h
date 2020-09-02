#ifndef UIATTRBOOLCONTROL_H
#define UIATTRBOOLCONTROL_H

#include "CustomCombineControl/UICustomSwitchControl.h"
#include "NDAttributeBase.h"
#include "NDBoolAttribute.h"
#include "nodeattributecontrol_global.h"
class NODEATTRIBUTECONTROLSHARED_EXPORT UIAttrBoolControl : public UICustomSwitchControl
{
    Q_OBJECT

public:
    UIAttrBoolControl(NDAttributeBase* attribute = nullptr, QWidget* parent = nullptr);
    ~UIAttrBoolControl();

    // 设置属性
    void setAttribute(NDAttributeBase* attribute);

private:
    NDBoolAttribute* m_attribute = nullptr;

private slots:
    void onAttrValueChanged(const QVariant& value);
    void onControlValueChanged(bool value, bool cmd = false);
};

#endif
