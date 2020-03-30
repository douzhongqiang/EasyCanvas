#include "UICustomSwitchWidget.h"
#include "UICustomCore/CustomStyleConfig.h"
#include <QPainter>

UICustomSwitchWidget::UICustomSwitchWidget(QWidget* parent)
    :CustomWidget(parent)
    ,m_margin(1, 1, 1, 1)
    ,m_cBgColor(223, 105, 20)
    ,m_cSliderColor(243, 125, 40)
    , m_cOnBgColor(155, 78, 15)
    , m_cOnSliderColor(226, 105, 5)
    , m_cOffBgColor(138, 138, 138)
    , m_cOffSliderColor(160, 160, 160)
{
    g_StyleConfig->setCurrentStyle(this, "Transparent");
    this->setCursor(Qt::PointingHandCursor);
    setSwitchStatus(true, false);

    m_pTimer = new QTimer;
    m_pTimer->setInterval(20);
    QObject::connect(m_pTimer, &QTimer::timeout, this, &UICustomSwitchWidget::onTimeout);
}

UICustomSwitchWidget::~UICustomSwitchWidget()
{

}

void UICustomSwitchWidget::setSwitchStatus(bool isOn, bool hasAnimation)
{
    if (isOn)
    {
        m_cBgColor = m_cOnBgColor;
        m_cSliderColor = m_cOnSliderColor;
    }
    else
    {
        m_cBgColor = m_cOffBgColor;
        m_cSliderColor = m_cOffSliderColor;
    }

    if (m_isSwitchOn != isOn)
    {
        m_isSwitchOn = isOn;
        emit valueChanged(isOn);
    }

    // 开启动画
    if (hasAnimation)
        m_pTimer->start();

    this->update();
}

bool UICustomSwitchWidget::getSwitchStatus(void)
{
    return m_isSwitchOn;
}

void UICustomSwitchWidget::customPaint(QPainter* painter)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QRect rect = this->rect();
    rect = rect.adjusted(m_margin.left(), m_margin.top(), -m_margin.right(), -m_margin.bottom());

    // 绘制背景
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(m_cBgColor));
    painter->drawRoundedRect(rect, 4, 4);

    // 绘制滑块
    painter->setBrush(QBrush(m_cSliderColor));
    qreal interval = (rect.height() - m_nSliderHeight) / 2.0;
    QRect sliderRect(interval + m_nCurrentXPos, rect.y() + interval, m_nSliderWidth, m_nSliderHeight);
    painter->drawRoundedRect(sliderRect, 4, 4);
    painter->restore();
}

QSize UICustomSwitchWidget::sizeHint() const
{
    QSize size(80, 30);
    return size;
}

void UICustomSwitchWidget::mousePressEvent(QMouseEvent* event)
{
    setSwitchStatus(!m_isSwitchOn);
}

void UICustomSwitchWidget::onTimeout(void)
{
    QRect rect = this->rect();
    rect = rect.adjusted(m_margin.left(), m_margin.top(), -m_margin.right(), -m_margin.bottom());
    qreal interval = (rect.height() - m_nSliderHeight) / 2.0;

    int startPos = 0;
    int endXPos = rect.right() - interval - m_nSliderWidth;
    if (m_isSwitchOn)
    {
        // 向左移动
        m_nCurrentXPos -= m_nAnimationInterval;
        if (m_nCurrentXPos <= startPos)
        {
            m_nCurrentXPos = 0;
            m_pTimer->stop();
        }
    }
    else
    {
        // 向右移动
        m_nCurrentXPos += m_nAnimationInterval;
        if (m_nCurrentXPos >= endXPos)
        {
            m_nCurrentXPos = endXPos;
            m_pTimer->stop();
        }
    }
    this->update();
}
