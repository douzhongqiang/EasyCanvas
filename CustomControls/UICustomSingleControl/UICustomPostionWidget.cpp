#include "UICustomPostionWidget.h"
#include "UICustomCore/CustomStyleConfig.h"
#include <QPainter>
#include <QMouseEvent>

UICustomPostionWidget::UICustomPostionWidget(QWidget* parent)
    :CustomWidget(parent)
{
    this->setMouseTracking(true);
    g_StyleConfig->setCurrentStyle(this, "Transparent");
}

UICustomPostionWidget::~UICustomPostionWidget()
{

}

void UICustomPostionWidget::customPaint(QPainter* painter)
{
    QPoint pos = transToThisPostion();

    painter->save();
    painter->setPen(QPen(g_StyleConfig->getHighLightColor()));
    painter->setRenderHint(QPainter::Antialiasing);

    // 绘制矩形
    QRect rect = this->rect().adjusted(m_nRadius, m_nRadius, -m_nRadius, -m_nRadius);
    painter->drawRect(rect);

    // 绘制位置
    painter->setPen(QPen(QColor(230, 230, 230)));
    painter->drawEllipse(pos, m_nRadius, m_nRadius);

    painter->restore();
}

QSize UICustomPostionWidget::sizeHint() const
{
    QSize size(150, 150);
    return size;
}

void UICustomPostionWidget::mousePressEvent(QMouseEvent* event)
{
    m_isPressed = true;

    QPoint pos = event->pos();
    transToThisValue(pos);
    this->update();
}

void UICustomPostionWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_isPressed)
        return;

    QPoint pos = event->pos();
    transToThisValue(pos);
    this->update();
}

void UICustomPostionWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (!m_isPressed)
        return;

    m_isPressed = false;

    QPoint pos = event->pos();
    transToThisValue(pos);
    this->update();
}

QPoint UICustomPostionWidget::transToThisPostion(void)
{
    QRect rect = this->rect().adjusted(m_nRadius, m_nRadius, -m_nRadius, -m_nRadius);

    qreal xPt = m_xValue / 100.0 * rect.width() + rect.x();
    qreal yPt = m_yValue / 100.0 * rect.height() + rect.y();

    QPoint pos;
    pos.setX(xPt);
    pos.setY(yPt);

    return pos;
}

void UICustomPostionWidget::transToThisValue(QPoint pos)
{
    QRect rect = this->rect().adjusted(m_nRadius, m_nRadius, -m_nRadius, -m_nRadius);
    if (!rect.contains(pos))
        return;

    m_xValue = (pos.x() - rect.x()) * 1.0 / rect.width() * 100;
    m_yValue = (pos.y() - rect.y()) * 1.0 / rect.height() * 100;
    emit postionValueChanged(m_xValue, m_yValue);
}

void UICustomPostionWidget::setPostionValue(qreal xValue, qreal yValue)
{
    m_xValue = xValue;
    m_yValue = yValue;

    this->update();
}

void UICustomPostionWidget::setPostionXValue(qreal xValue)
{
    m_xValue = xValue;
    this->update();
}

void UICustomPostionWidget::setPostionYValue(qreal yValue)
{
    m_yValue = yValue;
    this->update();
}

void UICustomPostionWidget::getPostionValue(qreal& xValue, qreal &yValue)
{
    xValue = m_xValue;
    yValue = m_yValue;
}

qreal UICustomPostionWidget::getPostionXValue(void)
{
    return m_xValue;
}

qreal UICustomPostionWidget::getPostionYValue(void)
{
    return m_yValue;
}
