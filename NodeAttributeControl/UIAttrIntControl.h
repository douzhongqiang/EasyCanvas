#ifndef UIATTRINTCONTROL_H
#define UIATTRINTCONTROL_H

#include "CustomCombineControl/UICustomIntControl.h"
#include "NDNodeBase.h"
#include "NDIntAttribute.h"
class UIAttrIntControl : public UICustomIntControl
{
    Q_OBJECT

public:
    UIAttrIntControl(NDAttributeBase* attribute = nullptr, QWidget* parent = nullptr);
    ~UIAttrIntControl();

    // 设置属性
    void setAttribute(NDAttributeBase* attribute);

private:
    NDIntAttribute* m_attribute = nullptr;

private slots:
    void onAttrValueChanged(const QVariant& value);
    void onControlValueChanged(int value, bool cmd = false);
};

#endif
