#ifndef UICUSTOMDATEEDITCONTROL_H
#define UICUSTOMDATEEDITCONTROL_H

#include "UICustomCombineControlBase.h"
#include "UICustomSingleControl/UICustomDateEdit.h"
#include "customcontrols_global.h"

class CUSTOMCONTROLSSHARED_EXPORT UICustomDateEditControl : public UICustomCombineControlBase
{
    Q_OBJECT

public:
    UICustomDateEditControl(QWidget* parent = nullptr);
    ~UICustomDateEditControl();

    // 设置/获取日期
    void setCurrentDate(const QDate& date);
    QDate getCurrentDate(void);

private:
    UICutomDateEdit* m_pDateEdit = nullptr;
};

#endif
