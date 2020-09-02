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
    this->setCurrentValue(m_attribute->getValue().toDouble());
    this->setTagText(m_attribute->getDisplayName());
}

void UIAttrFloatControl::onValueChanged(const QVariant& value)
{
    this->setCurrentValue(value.toDouble());
}

void UIAttrFloatControl::onControlValueChanged(qreal value, bool cmd)
{
    QObject::disconnect(m_attribute, &NDRealAttribute::valueChanged, this, &UIAttrFloatControl::onValueChanged);
    m_attribute->setValue(value, cmd);
    QObject::connect(m_attribute, &NDRealAttribute::valueChanged, this, &UIAttrFloatControl::onValueChanged);
}
