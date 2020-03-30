#ifndef UICUSTOMORIENTATIONCONTROL_H
#define UICUSTOMORIENTATIONCONTROL_H

#include "UICustomWidgtes/CustomWidget.h"
#include "UICustomCombineControlBase.h"
#include "customcontrols_global.h"

class UICustomOrientationItemWidget;
class UICustomOrientationWidget;
class CUSTOMCONTROLSSHARED_EXPORT UICustomOrientationControl : public UICustomCombineControlBase
{
    Q_OBJECT

public:
    UICustomOrientationControl(QWidget* parent = nullptr);
    ~UICustomOrientationControl();

private:
    UICustomOrientationWidget* m_pOrientationWidget = nullptr;
};

// -----------------------------------------------------------------
class UICustomOrientationItemWidget : public CustomWidget
{
    Q_OBJECT

public:
    UICustomOrientationItemWidget();
    ~UICustomOrientationItemWidget();

    void setPressedStatus(bool status);

protected:
    virtual void customPaint(QPainter* painter) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual QSize sizeHint() const override;

private:
    bool m_isPressed = false;
    QColor m_color;

signals:
    void clickedItem(void);
};

// -----------------------------------------------------------------
class UICustomOrientationWidget : public CustomWidget
{
    Q_OBJECT

public:
    UICustomOrientationWidget(QWidget* parent = nullptr);
    ~UICustomOrientationWidget();

private:
    QList<UICustomOrientationItemWidget*> m_items;

    void cleanAllItemsStatus(void);

private slots:
    void onClickedItem(void);
};

#endif
