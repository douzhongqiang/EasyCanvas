#ifndef UICUSTOMCOLORVIEWWIDGET_H
#define UICUSTOMCOLORVIEWWIDGET_H

#include "UICustomWidgtes/CustomWidget.h"
#include <QWidget>

class UICustomColorViewWidget : public CustomWidget
{
    Q_OBJECT

public:
    UICustomColorViewWidget(QWidget* parent = nullptr);
    ~UICustomColorViewWidget();

    // 设置/获取当前显示的颜色
    void setCurrentColor(const QColor& color);
    const QColor& getCurrentColor(void);

    // 设置是否允许拖拽
    void setDragVisible(bool isVisible);
    // 设置是否被选择
    void setSelected(bool isSelected);
    bool getSelected(void);

protected:
    virtual void customPaint(QPainter* painter) override;
    virtual QSize sizeHint() const override;
    virtual void mousePressEvent(QMouseEvent* event) override;

    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dragLeaveEvent(QDragLeaveEvent *event) override;
    virtual void dragMoveEvent(QDragMoveEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;

private:
    QColor m_currentColor;
    QColor m_selectedColor;
    bool m_isDragVisible = false;
    bool m_isSelected = false;

signals:
    void mousePressed(void);
    void dragEnterSignals(void);
    void dragReleaseSignals(void);
    void dropSignal(void);
};

#endif
