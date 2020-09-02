#include "UICustomDoubleControl.h"
#include "UICustomSingleControl/UICustomDoubleSpinBox.h"

UICustomDoubleControl::UICustomDoubleControl(QWidget* parent)
    :UICustomCombineControlBase(parent)
{
    // Double
    m_pDoubleSpinBox = new UICustomDoubleSpinBox;
    m_pMainLayout->addWidget(m_pDoubleSpinBox);

    m_pSlider = new QSlider(Qt::Horizontal);
    m_pSlider->setMinimum(0);
    m_pSlider->setMaximum(100);
    m_pMainLayout->addWidget(m_pSlider);

    setRangeValue(0, 1.0);
    m_pSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QObject::connect(m_pDoubleSpinBox, &UICustomDoubleSpinBox::editingFinished, \
                     this, &UICustomDoubleControl::onDoubleValueChanged);

    QObject::connect(m_pSlider, &QSlider::valueChanged, this, &UICustomDoubleControl::onSliderValueChanged);
    QObject::connect(m_pSlider, &QSlider::sliderPressed, this, &UICustomDoubleControl::onSliderPressed);
    QObject::connect(m_pSlider, &QSlider::sliderMoved, this, &UICustomDoubleControl::onSliderMoved);
    QObject::connect(m_pSlider, &QSlider::sliderReleased, this, &UICustomDoubleControl::onSliderReleased);

    this->setWidth(400);
    this->setHeight(30);
}

UICustomDoubleControl::~UICustomDoubleControl()
{

}

void UICustomDoubleControl::setCurrentValue(qreal value)
{
    if (value < m_qMinValue || value > m_qMaxValue)
        return;

    m_pDoubleSpinBox->setValue(value);
    int val = (value - m_qMinValue) * 1.0 / (m_qMaxValue - m_qMinValue) * 100;

    m_pSlider->blockSignals(true);
    m_pSlider->setValue(val);
    m_pSlider->blockSignals(false);
}

qreal UICustomDoubleControl::getCurrentValue(void)
{
    return m_pDoubleSpinBox->value();
}

void UICustomDoubleControl::setRangeValue(qreal minValue, qreal maxValue)
{
    m_qMinValue = minValue;
    m_qMaxValue = maxValue;

    m_pDoubleSpinBox->setMinimum(m_qMinValue);
    m_pDoubleSpinBox->setMaximum(m_qMaxValue);
}

void UICustomDoubleControl::onSliderPressed(void)
{
    m_beforeValue = m_pDoubleSpinBox->value();

    qreal valus = getValuesBySlider();
    m_pDoubleSpinBox->setValue(valus);

    emit valueChanged(valus);
}

void UICustomDoubleControl::onSliderMoved(void)
{
    qreal valus = getValuesBySlider();
    m_pDoubleSpinBox->setValue(valus);

    emit valueChanged(valus);
}

void UICustomDoubleControl::onSliderReleased(void)
{
    qreal valus = getValuesBySlider();
    m_pDoubleSpinBox->setValue(valus);

    emit valueChanged(m_beforeValue);
    emit valueChanged(valus, true);
}

void UICustomDoubleControl::onSliderValueChanged(int value)
{
    qreal valus = getValuesBySlider();
    m_pDoubleSpinBox->setValue(valus);

    emit valueChanged(valus);
}

qreal UICustomDoubleControl::getValuesBySlider(void)
{
    int value = m_pSlider->value();
    return m_qMinValue + (m_qMaxValue - m_qMinValue) * value * 1.0 / 100;
}

void UICustomDoubleControl::onDoubleValueChanged(void)
{
    qreal value = m_pDoubleSpinBox->value();
    int val = (value - m_qMinValue) * 1.0 / (m_qMaxValue - m_qMinValue) * 100;

    m_pSlider->blockSignals(true);
    m_pSlider->setValue(val);
    m_pSlider->blockSignals(false);

    emit valueChanged(value, true);
}
