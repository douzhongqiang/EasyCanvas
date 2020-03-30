#include "UICustomImageViewer.h"
#include <QPainter>
#include <QWheelEvent>
#include <QMouseEvent>

UICustomImageViewer::UICustomImageViewer(QWidget* parent)
    :QWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

UICustomImageViewer::~UICustomImageViewer()
{

}

void UICustomImageViewer::loadImage(const QString& imagePath)
{
    QImage image(imagePath);
    m_pixmap = QPixmap::fromImage(image);

    this->update();
}

void UICustomImageViewer::loadImage(const QImage& image)
{
    m_pixmap = QPixmap::fromImage(image);

    this->update();
}

void UICustomImageViewer::paintEvent(QPaintEvent* event)
{
    if (m_pixmap.isNull())
        return;

    QPainter npainter(this);
    QPainter* painter = &npainter;

    QRect rect = this->rect().adjusted(m_nMargin, m_nMargin, -m_nMargin, -m_nMargin);

    painter->save();

    // 平移
    painter->translate(QPoint(rect.center().x() + m_nIntervalX, rect.center().y() + m_nIntervalY));
    // 缩放
    painter->scale(m_zoomRate, m_zoomRate);

    // 计算实际显示大小
    int imageWidth = m_pixmap.width();
    imageWidth = qMin(rect.width(), imageWidth);
    int imageHeight = imageWidth * 1.0 / (m_pixmap.width() * 1.0 / m_pixmap.height());
    imageHeight = qMin(rect.height(), imageHeight);
    imageWidth = imageHeight * (m_pixmap.width() * 1.0 / m_pixmap.height());


    QRect pixRect(-imageWidth / 2, -imageHeight / 2, imageWidth, imageHeight);
    painter->drawPixmap(pixRect, m_pixmap);

    painter->restore();
}

void UICustomImageViewer::wheelEvent(QWheelEvent *event)
{
    int value = event->delta();
    if (value > 0)
        m_zoomRate += 0.2;
    else
        m_zoomRate -= 0.2;

    if (m_zoomRate < 0.5)
        m_zoomRate = 0.5;

    this->update();
}

void UICustomImageViewer::mousePressEvent(QMouseEvent* event)
{
    m_isMousePressed = true;
    m_tempPos = event->pos();
}

void UICustomImageViewer::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_isMousePressed)
        return;

    this->setCursor(Qt::SizeAllCursor);
    QPoint pos = event->pos();
    m_nIntervalX += pos.x() - m_tempPos.x();
    m_nIntervalY += pos.y() - m_tempPos.y();

    m_tempPos = pos;

    this->update();
}

void UICustomImageViewer::mouseReleaseEvent(QMouseEvent* event)
{
    m_isMousePressed = false;
    this->setCursor(Qt::ArrowCursor);
}

QSize UICustomImageViewer::sizeHint() const
{
    QSize size(600, 600);
    return size;
}

// 缩放
void UICustomImageViewer::zoom(qreal value)
{
    m_zoomRate = value;
    this->update();
}

qreal UICustomImageViewer::getZoomValue(void)
{
    return m_zoomRate;
}

// 复位
void UICustomImageViewer::rest(void)
{
    m_zoomRate = 1.0;
    m_nIntervalX = 0;
    m_nIntervalY = 0;

    this->update();
}
