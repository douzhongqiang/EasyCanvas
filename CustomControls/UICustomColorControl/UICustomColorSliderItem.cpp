#include "UICustomColorSliderItem.h"
#include <QPainter>
#include <QMouseEvent>

UICustomColorSliderItem::UICustomColorSliderItem(QWidget* parent)
    :CustomWidget(parent)
    , m_cStartColor(255, 0, 0)
    , m_cEndColor(0, 255, 0)
{
    m_colorVec  << QColor(255, 0, 0) << QColor(255, 255, 0) \
                << QColor(0, 255, 0) << QColor(0, 255, 255) \
                << QColor(0, 0, 255) << QColor(255, 0, 255);
}

UICustomColorSliderItem::~UICustomColorSliderItem()
{

}

void UICustomColorSliderItem::customPaint(QPainter* painter)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    QRect rect = this->rect().adjusted(m_nMargin + m_nSliderWidth / 2 - m_nInterVal, m_nMargin, \
                                       -(m_nMargin + m_nSliderWidth / 2 - m_nInterVal), \
                                       -m_nMargin - m_nSliderHeight);

    // 绘制边框
    painter->setPen(QPen(QColor(200, 200, 200)));
    painter->drawRoundedRect(rect, 4, 4);

    // 绘制渐变
    rect = rect.adjusted(m_nInterVal, m_nInterVal, -m_nInterVal, -m_nInterVal);
    QLinearGradient linerGradient(rect.topLeft(), rect.topRight());
    if (m_isHValueSlider)
    {
        for (int i=0; i<m_colorVec.size(); ++i)
        {
            qreal value = 1.0 / m_colorVec.size() * i;
            linerGradient.setColorAt(value, m_colorVec[i]);
        }
    }
    else
    {
        linerGradient.setColorAt(0, m_cStartColor);
        linerGradient.setColorAt(1, m_cEndColor);
    }

    painter->setPen(Qt::NoPen);
    painter->setBrush(linerGradient);
    painter->drawRoundedRect(rect, 4, 4);

    // 绘制滑动三角形手柄
    painter->setBrush(QColor(220, 220, 220));
    qreal xPt = m_nCurrentValue * 1.0 / (m_nMaxValue - m_nMinValue) * rect.width() + rect.x();
    QPoint currentPos(xPt, rect.bottom() + m_nInterVal + 1);
    QPolygon polygon;
    polygon << currentPos << QPoint(currentPos.x() - m_nSliderWidth / 2, currentPos.y() + m_nSliderHeight) \
            << QPoint(currentPos.x() + m_nSliderWidth / 2, currentPos.y() + m_nSliderHeight);
    painter->drawPolygon(polygon);

    painter->restore();
}

void UICustomColorSliderItem::mousePressEvent(QMouseEvent* event)
{
    QRect rect = this->rect().adjusted(m_nMargin + m_nSliderWidth / 2 - m_nInterVal, m_nMargin, \
                                       -(m_nMargin + m_nSliderWidth / 2 - m_nInterVal), \
                                       -m_nMargin - m_nSliderHeight);
    rect = rect.adjusted(m_nInterVal, m_nInterVal, -m_nInterVal, -m_nInterVal);
    qreal xPt = m_nCurrentValue * 1.0 / (m_nMaxValue - m_nMinValue) * rect.width() + rect.x();
    QPoint currentPos(xPt, rect.bottom() + m_nInterVal + 1);
    QPolygon polygon;
    polygon << currentPos << QPoint(currentPos.x() - m_nSliderWidth / 2, currentPos.y() + m_nSliderHeight) \
            << QPoint(currentPos.x() + m_nSliderWidth / 2, currentPos.y() + m_nSliderHeight);

    // 判断是否选中
    m_isSelected = false;
    QPoint pos = event->pos();
    if (rect.contains(pos))
        m_isSelected = true;
    else if (polygon.containsPoint(pos, Qt::OddEvenFill))
        m_isSelected = true;

    if (m_isSelected)
        selectedPoint(pos);
}

void UICustomColorSliderItem::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();
    if (m_isSelected)
        selectedPoint(pos);
}

void UICustomColorSliderItem::mouseReleaseEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();
    if (m_isSelected)
        selectedPoint(pos);

    m_isSelected = false;

}

QSize UICustomColorSliderItem::sizeHint() const
{
    QSize size(200, 30);
    return size;
}

void UICustomColorSliderItem::selectedPoint(const QPoint& pos)
{
    QRect rect = this->rect().adjusted(m_nMargin + m_nSliderWidth / 2 - m_nInterVal, m_nMargin, \
                                       -(m_nMargin + m_nSliderWidth / 2 - m_nInterVal), \
                                       -m_nMargin - m_nSliderHeight);
    rect = rect.adjusted(m_nInterVal, m_nInterVal, -m_nInterVal, -m_nInterVal);

    int value = (pos.x() - rect.x()) * 1.0 / rect.width() * (m_nMaxValue - m_nMinValue) + m_nMinValue;
    value = qMax(value, m_nMinValue);
    value = qMin(value, m_nMaxValue);
    m_nCurrentValue = value;

    // 发送信号
    emit changedValue(m_nCurrentValue);

    this->update();
}

// 设置开始颜色
void UICustomColorSliderItem::setStartColor(const QColor& startColor)
{
    m_cStartColor = startColor;
    this->update();
}

// 设置结束颜色
void UICustomColorSliderItem::setEndColor(const QColor& endColor)
{
    m_cEndColor = endColor;
    this->update();
}

// 设置当前选中的值
void UICustomColorSliderItem::setCurrentValue(int value)
{
    m_nCurrentValue = value;
    this->update();
}

// 获取当前选中值
int UICustomColorSliderItem::getCurrentValue(void)
{
    return m_nCurrentValue;
}

void UICustomColorSliderItem::setHVisible(bool isVisible)
{
    m_isHValueSlider = isVisible;
}
