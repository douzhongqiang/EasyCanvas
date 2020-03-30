#ifndef UICUSTOMIMAGEVIEWER_H
#define UICUSTOMIMAGEVIEWER_H

#include <QWidget>
#include "UICustomWidgtes/CustomWidget.h"
#include "customcontrols_global.h"

class CUSTOMCONTROLSSHARED_EXPORT UICustomImageViewer : public QWidget
{
    Q_OBJECT

public:
    UICustomImageViewer(QWidget* parent = nullptr);
    ~UICustomImageViewer();

    // 加载图片
    void loadImage(const QString& imagePath);
    void loadImage(const QImage& image);

    // 缩放
    void zoom(qreal value);
    qreal getZoomValue(void);

    // 复位
    void rest(void);

protected:
    void paintEvent(QPaintEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    QSize sizeHint() const override;

private:
    QPixmap m_pixmap;
    int m_nMargin = 2;

    // 缩放系数
    qreal m_zoomRate = 1.0;
    // 移动
    int m_nIntervalX = 0;
    int m_nIntervalY = 0;
    bool m_isMousePressed = false;
    QPoint m_tempPos;
};

#endif
