#include "UICustomPostionControl.h"

UICustomPostionControl::UICustomPostionControl(QWidget* parent)
    :UICustomCombineControlBase(parent)
{
    m_pPostionWidget = new UICustomPostionWidget;
    m_pMainLayout->addWidget(m_pPostionWidget);

    this->setWidth(400);
    this->setHeight(150);

    QObject::connect(m_pPostionWidget, &UICustomPostionWidget::postionValueChanged, \
                     this, &UICustomPostionControl::postionValueChanged);
}

UICustomPostionControl::~UICustomPostionControl()
{

}

// 设置当前映射的尺寸
void UICustomPostionControl::setCurrentMapSize(int width, int height)
{
    m_nMapWidth = width;
    m_nMapHeight = height;
}

// 设置当前的位置
void UICustomPostionControl::setCurrentXPt(int xValue)
{
    qreal val = xValue * 1.0 / m_nMapWidth * 100;
    m_pPostionWidget->setPostionXValue(val);
}

void UICustomPostionControl::setCurrentYPt(int yValue)
{
    qreal val = yValue * 1.0 / m_nMapHeight * 100;
    m_pPostionWidget->setPostionYValue(val);
}

void UICustomPostionControl::setCurrentPostion(int xValue, int yValue)
{
    qreal xVal = xValue * 1.0 / m_nMapWidth * 100;
    qreal yVal = yValue * 1.0 / m_nMapHeight * 100;
    m_pPostionWidget->setPostionValue(xVal, yVal);
}

// 获取当前位置
int UICustomPostionControl::getCurrentXPt(void)
{
    qreal xVal = m_pPostionWidget->getPostionXValue();
    return xVal / 100.0 * m_nMapWidth;
}

int UICustomPostionControl::getCurrentYPt(void)
{
    qreal yVal = m_pPostionWidget->getPostionYValue();
    return yVal / 100.0 * m_nMapHeight;
}

void UICustomPostionControl::getCurrentPostion(int& xValue, int& yValue)
{
    qreal xVal, yVal;
    m_pPostionWidget->getPostionValue(xVal, yVal);

    xValue = xVal / 100.0 * m_nMapWidth;
    yValue = yVal / 100.0 * m_nMapHeight;
}
