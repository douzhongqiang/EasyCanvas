#ifndef UICUSTOMPOSTIONWIDGET_H
#define UICUSTOMPOSTIONWIDGET_H

#include "UICustomWidgtes/CustomWidget.h"
#include "customcontrols_global.h"
#include <QWidget>
class CUSTOMCONTROLSSHARED_EXPORT UICustomPostionWidget : public CustomWidget
{
    Q_OBJECT

public:
    UICustomPostionWidget(QWidget* parent = nullptr);
    ~UICustomPostionWidget();

    // 设置位置
    void setPostionValue(qreal xValue, qreal yValue);
    void setPostionXValue(qreal xValue);
    void setPostionYValue(qreal yValue);

    // 获取位置
    void getPostionValue(qreal& xValue, qreal &yValue);
    qreal getPostionXValue(void);
    qreal getPostionYValue(void);

protected:
    // Custom Paint Event
    void customPaint(QPainter* painter) override;
    QSize sizeHint() const override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    bool m_isPressed = false;
    int m_nRadius = 4;

    qreal m_xValue = 0;     // 百分比值
    qreal m_yValue = 0;
    QPoint transToThisPostion(void);
    void transToThisValue(QPoint pos);

signals:
    void postionValueChanged(qreal xValue, qreal yValue);
};

#endif
