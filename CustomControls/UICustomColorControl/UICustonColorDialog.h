#ifndef UICUSTOMCONTROLDIALOG_H
#define UICUSTOMCONTROLDIALOG_H

#include "UICustomWidgtes/CustomDialog.h"
#include "UICustomColorWidget.h"
#include <QWidget>

class UICustomColorDialog : public CustomDialog
{
    Q_OBJECT

public:
    UICustomColorDialog(QWidget* parent = nullptr);
    ~UICustomColorDialog();

    // 设置/获取颜色
    void setCurrentColor(const QColor& color);
    QColor getCurrentColor(void);

private:
    UICustomColorWidget* m_pColorWidget = nullptr;

    void init(void);
};

#endif
