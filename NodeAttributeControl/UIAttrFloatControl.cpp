#include "UIAttrFloatControl.h"
#include <QDebug>

UIAttrFloatControl::UIAttrFloatControl(NDAttributeBase* attribute, QWidget* parent)
    :UICustomDoubleControl(parent)
{
    setAttribute(attribute);

    if (m_attribute == nullptr)
        return;

    QObject::connect(m_attribute, &NDRealAttribute::valueChanged, this, &UIAttrFloatControl::onValueChanged);
    QObject::connect(this, &UIAttrFloatControl::valueChanged, this, &UIAttrFloatControl::onControlValueChanged);
}

UIAttrFloatControl::~UIAttrFloatControl()
{

}

void UIAttrFloatControl::setAttribute(NDAttributeBase* attribute)
{
    if (attribute == nullptr || attribute->Type() != NDAttributeBase::t_qreal)
        return;

    m_attribute = qobject_cast<NDRealAttribute*>(attribute);
    if (m_attribute == nullptr)
        return;

    // 设置范围
    qreal startValue, endValue;
    m_attribute->getValueRange(startValue, endValue);
    this->setRangeValue(startValue, endValue);

    // 设置值
    this->setCurrentValue(m_attribute->getCurrentValue());
    this->setTagText(m_attribute->getDisplayName());
}

void UIAttrFloatControl::onValueChanged(qreal value)
{
    this->setCurrentValue(value);
}

void UIAttrFloatControl::onControlValueChanged(qreal value)
{
    QObject::disconnect(m_attribute, &NDRealAttribute::valueChanged, this, &UIAttrFloatControl::onValueChanged);
    m_attribute->setCurrentValue(value);
    QObject::connect(m_attribute, &NDRealAttribute::valueChanged, this, &UIAttrFloatControl::onValueChanged);
}
