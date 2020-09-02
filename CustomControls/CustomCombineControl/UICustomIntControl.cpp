#include "UICustomIntControl.h"
#include "UICustomSingleControl/UICustomIntSpinBox.h"
#include <QDebug>

UICustomIntControl::UICustomIntControl(QWidget* parent)
    :UICustomCombineControlBase(parent)
{
    m_pIntValue = new UICustomIntSpinBox;
    m_pMainLayout->addWidget(m_pIntValue);

    m_pSlider = new QSlider(Qt::Horizontal);
    m_pMainLayout->addWidget(m_pSlider);
    m_pSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    setRangeValue(0, 100);
    m_pSlider->setMinimum(0);
    m_pSlider->setMaximum(100);

    QObject::connect(m_pIntValue, &UICustomIntSpinBox::editingFinished, \
                     this, &UICustomIntControl::onIntValueChanged);

    QObject::connect(m_pSlider, &QSlider::valueChanged, this, &UICustomIntControl::onSliderValueChanged);
    QObject::connect(m_pSlider, &QSlider::sliderPressed, this, &UICustomIntControl::onSliderPressed);
    QObject::connect(m_pSlider, &QSlider::sliderMoved, this, &UICustomIntControl::onSliderMoved);
    QObject::connect(m_pSlider, &QSlider::sliderReleased, this, &UICustomIntControl::onSliderReleased);

    this->setWidth(400);
    this->setHeight(30);
}

UICustomIntControl::~UICustomIntControl()
{

}

void UICustomIntControl::setRangeValue(int minValue, int maxValue)
{
    m_nMaxValue = maxValue;
    m_nMinValue = minValue;

    m_pIntValue->setMinimum(m_nMinValue);
    m_pIntValue->setMaximum(m_nMaxValue);
}


void UICustomIntControl::setCurrentValue(int value)
{
    if (value < m_nMinValue || value > m_nMaxValue)
        return;

    m_pIntValue->setValue(value);

    int val = (value - m_nMinValue) * 1.0 / (m_nMaxValue - m_nMinValue) * 100;

    m_pSlider->blockSignals(true);
    m_pSlider->setValue(val);
    m_pSlider->blockSignals(false);
}

int UICustomIntControl::getCurrentValue(void)
{
    return m_pIntValue->value();
}

int UICustomIntControl::getValuesBySlider(void)
{
    int value = m_pSlider->value();
    return m_nMinValue + value * 1.0 / 100 * (m_nMaxValue - m_nMinValue);
}

void UICustomIntControl::onSliderPressed(void)
{
    m_tempValue = m_pIntValue->value();

    int value = getValuesBySlider();
    m_pIntValue->setValue(value);

    emit valueChanged(value);
}

void UICustomIntControl::onSliderMoved(void)
{
    int value = getValuesBySlider();
    m_pIntValue->setValue(value);

    emit valueChanged(value);
}

void UICustomIntControl::onSliderReleased(void)
{
    int value = getValuesBySlider();
    m_pIntValue->setValue(value);

    emit valueChanged(m_tempValue);
    emit valueChanged(value, true);
}

void UICustomIntControl::onSliderValueChanged(int val)
{
    int value = getValuesBySlider();
    m_pIntValue->setValue(value);

    emit valueChanged(value);
}

void UICustomIntControl::onIntValueChanged(void)
{
    int value = m_pIntValue->value();
    m_pSlider->blockSignals(true);
    m_pSlider->setValue(value * 1.0 / (m_nMaxValue - m_nMinValue) * 100);
    m_pSlider->blockSignals(false);

    emit valueChanged(value, true);
}
