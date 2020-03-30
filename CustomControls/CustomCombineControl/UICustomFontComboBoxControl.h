#ifndef UICUSTOMFONTCOMBOBOXCONTROL_H
#define UICUSTOMFONTCOMBOBOXCONTROL_H

#include "UICustomCombineControlBase.h"
#include "customwidgets_global.h"
#include <QWidget>

class QFontComboBox;
class CUSTOMCONTROLSSHARED_EXPORT UICustomFontComboBoxControl : public UICustomCombineControlBase
{
    Q_OBJECT

public:
    UICustomFontComboBoxControl(QWidget* parent = nullptr);
    ~UICustomFontComboBoxControl();

private:
    QFontComboBox* m_pComboBox = nullptr;
};

#endif
