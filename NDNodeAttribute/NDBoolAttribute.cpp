#include "NDBoolAttribute.h"

NDBoolAttribute::NDBoolAttribute(NDNodeBase *parentNode)
    :NDAttributeBase(parentNode)
{

}

NDBoolAttribute::~NDBoolAttribute()
{

}

// 设置/获取当前值
void NDBoolAttribute::setCurrentValue(bool value)
{
    bool needSendSignal = true;
    if (m_value == value)
        needSendSignal = false;

    m_value = value;

    if (needSendSignal)
        emit valueChanged(value);
}

bool NDBoolAttribute::getCurrentValue(void)
{
    return m_value;
}
