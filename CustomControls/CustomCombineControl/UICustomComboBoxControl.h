#ifndef UICUSTOMCOMBOBOXCONTROL_H
#define UICUSTOMCOMBOBOXCONTROL_H

#include "UICustomCombineControlBase.h"
#include "UICustomSingleControl/UICustomComboBox.h"
#include "customcontrols_global.h"

class CUSTOMCONTROLSSHARED_EXPORT UICustomComboBoxControl : public UICustomCombineControlBase
{
    Q_OBJECT

public:
    UICustomComboBoxControl(QWidget* parent = nullptr);
    ~UICustomComboBoxControl();

    // 添加元素
    void addItem(const QString& item);
    void addItem(const QString& name, const QVariant& data);
    // 获取当前元素
    void setCurrentItem(const QString& text);
    QString getCurrentItem(void);
    void setCurrentIndex(int index);
    int getCurrentIndex(void);

    // 获取当前数据
    QVariant getCurrentItemData(void);
    // 获取当前个数
    int getItemCount(void);

    // 清除所有元素
    void clearAllItems(void);

private:
    UICustomComboBox* m_pCustomComboBox = nullptr;

signals:
    void currentTextChanged(const QString& str);
    void currentIndexChanged(int index);
};

#endif
