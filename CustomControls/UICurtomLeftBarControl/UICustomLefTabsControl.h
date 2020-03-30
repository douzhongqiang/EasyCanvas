#ifndef UICUSTOMLEFTTABSCONTROL_H
#define UICUSTOMLEFTTABSCONTROL_H

#include "UICustomWidgtes/CustomWidget.h"
#include <QWidget>
#include <QStackedWidget>
#include "customcontrols_global.h"

class UICutomLeftWidget;
class CUSTOMCONTROLSSHARED_EXPORT UICustomLeftTabsControl : public CustomWidget
{
    Q_OBJECT

public:
    UICustomLeftTabsControl(QWidget* parent = nullptr);
    ~UICustomLeftTabsControl();

    // 添加元素
    void addWidget(QWidget* widget, const QString& name, const QString& iconPath = "");
    // 设置当前选中的Item
    void setCurrentSelectedItem(int index);

private:
    QStackedWidget* m_pStackWidget = nullptr;
    UICutomLeftWidget* m_pLeftWidget = nullptr;

private slots:
    void onClickedItem(int index);
};

#endif
