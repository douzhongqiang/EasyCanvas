#ifndef UICUSTOMLEFTWIDGET_H
#define UICUSTOMLEFTWIDGET_H

#include "UICustomWidgtes/CustomWidget.h"
#include <QWidget>

class QVBoxLayout;
class UICustomLeftItemWidget;
class UICutomLeftWidget : public CustomWidget
{
    Q_OBJECT

public:
    UICutomLeftWidget(QWidget* parent = nullptr);
    ~UICutomLeftWidget();

    // 添加元素
    void addItem(const QString& name, const QString& iconPath = "");
    // 清除所有选中
    void cleanAllSelected(void);
    // 设置当前选中
    void setCurrentSelected(int index);

private:
    QVBoxLayout* m_pMainLayout = nullptr;
    QList<UICustomLeftItemWidget*> m_items;

private slots:
    void onClickedItem(void);

signals:
    void clickedItem(int);
};

#endif
