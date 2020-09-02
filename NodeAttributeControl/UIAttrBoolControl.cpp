#include "UIAttrBoolControl.h"

UIAttrBoolControl::UIAttrBoolControl(NDAttributeBase* attribute, QWidget* parent)
    :UICustomSwitchControl(parent)
{
    setAttribute(attribute);
}

UIAttrBoolControl::~UIAttrBoolControl()
{

}

void UIAttrBoolControl::setAttribute(NDAttributeBase* attribute)
{
    if (attribute == nullptr || attribute->Type() != NDAttributeBase::t_bool)
        return;

    m_attribute = qobject_cast<NDBoolAttribute*>(attribute);
    this->setCurrentValue(m_attribute->getValue().toBool());
    this->setTagText(m_attribute->getDisplayName());

    QObject::connect(m_attribute, &NDBoolAttribute::valueChanged, this, &UIAttrBoolControl::onAttrValueChanged);
    QObject::connect(this, &UIAttrBoolControl::valueChanged, this, &UIAttrBoolControl::onControlValueChanged);
}

void UIAttrBoolControl::onAttrValueChanged(const QVariant& value)
{
    this->setCurrentValue(value.toBool(), false);
}

void UIAttrBoolControl::onControlValueChanged(bool value, bool cmd)
{
    QObject::disconnect(m_attribute, &NDBoolAttribute::valueChanged, this, &UIAttrBoolControl::onAttrValueChanged);
    m_attribute->setValue(value, cmd);
    QObject::connect(m_attribute, &NDBoolAttribute::valueChanged, this, &UIAttrBoolControl::onAttrValueChanged);
}
