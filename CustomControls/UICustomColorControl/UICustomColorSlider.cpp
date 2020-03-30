#include "UICustomColorSlider.h"

UICustomColorSlider::UICustomColorSlider(QWidget *parent)
    :UICustomCombineControlBase(parent)
{
    m_pColorSliderItem = new UICustomColorSliderItem;
    m_pValueSpixBox = new UICustomIntSpinBox;

    m_pMainLayout->addWidget(m_pValueSpixBox);
    m_pMainLayout->addWidget(m_pColorSliderItem);
    m_pColorSliderItem->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setColumnWidth(0, 40);

    QObject::connect(m_pColorSliderItem, &UICustomColorSliderItem::changedValue, \
                     this, &UICustomColorSlider::onChangedValue);
    QObject::connect(m_pColorSliderItem, &UICustomColorSliderItem::changedValue, \
                     this, &UICustomColorSlider::valueChanged);

    QObject::connect(m_pValueSpixBox, &UICustomIntSpinBox::editingFinished, [&](void){
        emit valueChanged(m_pValueSpixBox->value());
    });
}

UICustomColorSlider::~UICustomColorSlider()
{

}

void UICustomColorSlider::setValueRange(int minValue, int maxValue)
{
    m_pValueSpixBox->setMinimum(minValue);
    m_pValueSpixBox->setMaximum(maxValue);

    m_nMinValue = minValue;
    m_nMaxValue = maxValue;
}

// 设置开始颜色
void UICustomColorSlider::setStartColor(const QColor& startColor)
{
    m_pColorSliderItem->setStartColor(startColor);
}

// 设置结束颜色
void UICustomColorSlider::setEndColor(const QColor& endColor)
{
    m_pColorSliderItem->setEndColor(endColor);
}

void UICustomColorSlider::onChangedValue(int value)
{
    int val = (m_nMaxValue - m_nMinValue) * value * 1.0 / 100;
    m_pValueSpixBox->setValue(val);
}

// 设置当前值
void UICustomColorSlider::setCurrentValue(int value)
{
    if (value < m_nMinValue || value > m_nMaxValue)
        return;

    m_pValueSpixBox->setValue(value);
    int val = (value - m_nMinValue) * 1.0 / (m_nMaxValue - m_nMinValue) * 100;
    m_pColorSliderItem->setCurrentValue(val);
}

// 获取当前值
int UICustomColorSlider::getCurrentValue(void)
{
    return m_pValueSpixBox->value();
}

// 设置是否为H
void UICustomColorSlider::setHVisible(bool isVisible)
{
    m_pColorSliderItem->setHVisible(isVisible);
}
