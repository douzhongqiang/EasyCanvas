#ifndef UICUSTOMCOLORRANGE_H
#define UICUSTOMCOLORRANGE_H

#include "UICustomWidgtes/CustomWidget.h"
class UICustomColorCircle : public CustomWidget
{
    Q_OBJECT

public:
    UICustomColorCircle(QWidget* parent = nullptr);
    ~UICustomColorCircle();

    enum SelectedMode
    {
        Normal,     // 未选中状态
        Circle,     // 圆环选中状态
        Rect        // 矩形区域选中状态
    };

    // 设置/获取当前颜色
    QColor getCurrentColor(void);
    void setCurrentColor(const QColor& color);

protected:
    void customPaint(QPainter* painter) override;
    void resizeEvent(QResizeEvent* event) override;
    QSize sizeHint() const override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    int m_nCircleWidth = 30;
    int m_nHValue = 0;
    QVector<QColor> m_colorVec;
    SelectedMode m_selectMode = Normal;
    QColor m_currentColor;

    // 绘制中心矩形
    void drawCenterRect(const QRect& rect, QPainter* painter);
    // 获取两点之间的距离
    qreal distance(QPoint p1, QPoint p2);
    // 根据点的位置或者角度
    int getHValue(QPoint pos);
    // 更新当前颜色值显示
    void updateCurrentColor(QPoint pos);

signals:
    void colorChanged(void);
};
#endif
