#ifndef UICUSTOMSWITCHCONTROL_H
#define UICUSTOMSWITCHCONTROL_H

#include "UICustomCombineControlBase.h"
#include "UICustomSingleControl/UICustomSwitchWidget.h"
#include "customcontrols_global.h"
#include <QLabel>

class CUSTOMCONTROLSSHARED_EXPORT UICustomSwitchControl : public UICustomCombineControlBase
{
    Q_OBJECT

public:
    UICustomSwitchControl(QWidget* parent = nullptr);
    ~UICustomSwitchControl();

    // 设置/获取当前值
    void setCurrentValue(bool isOn, bool hasAnim = true);
    bool getCurrentValue(void);

    // 设置右侧文本
    void setRightText(const QString& text);

private:
    UICustomSwitchWidget* m_pSwitchWidget = nullptr;
    QLabel* m_pLabel = nullptr;

signals:
    void valueChanged(bool isOn, bool cmd = false);
};

#endif
