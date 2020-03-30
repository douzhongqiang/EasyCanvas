#ifndef UICUSTOMLEFTITEMWIDGET_H
#define UICUSTOMLEFTITEMWIDGET_H

#include "UICustomWidgtes/CustomWidget.h"
#include "customcontrols_global.h"
#include <QWidget>

class CUSTOMCONTROLSSHARED_EXPORT UICustomLeftItemWidget : public CustomWidget
{
    Q_OBJECT
public:
    UICustomLeftItemWidget(QWidget* parent = nullptr);
    ~UICustomLeftItemWidget();

    // 设置/获取是否被选中
    void setSelected(bool isSelected);
    bool isSelected(void);

    // 设置/获取字符串
    void setText(const QString& name);
    QString getText(void);

    // 设置图标
    void setIcon(const QString& imageFile);

protected:
    void customPaint(QPainter* painter) override;
    void mousePressEvent(QMouseEvent* event) override;
    QSize sizeHint(void) const override;

    bool m_isSelected = false;
    QString m_cString;
    QPixmap m_pixmap;

    int m_nLeftSeletedWidth = 5;
    int m_nInterval = 2;
    int m_nArrowWidth = 4;

signals:
    void clickedItem(void);
};

#endif
