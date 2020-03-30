#ifndef UICUSTOMTIMEEDITCONTROL_H
#define UICUSTOMTIMEEDITCONTROL_H

#include "UICustomCombineControlBase.h"
#include "UICustomSingleControl/UICustomTimeEdit.h"
#include "customcontrols_global.h"
#include <QWidget>

class CUSTOMCONTROLSSHARED_EXPORT UICustomTimeEditControl : public UICustomCombineControlBase
{
    Q_OBJECT

public:
    UICustomTimeEditControl(QWidget* parent = nullptr);
    ~UICustomTimeEditControl();

    // 设置/获取当前时间
    void setCurrentTime(const QTime& time);
    QTime getCurrentTime(void);

private:
    UICustomTimeEdit* m_pTimeEdit = nullptr;
};

#endif
