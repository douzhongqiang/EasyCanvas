#include "NDColorAttribute.h"

NDColorAttribute::NDColorAttribute(NDNodeBase *parentNode)
    :NDAttributeBase(parentNode)
{

}

NDColorAttribute::~NDColorAttribute()
{

}

// 设置/获取当前颜色
void NDColorAttribute::setCurrentValue(const QColor& value)
{
    if (m_nValue != value)
    {
        m_nValue = value;
        emit valueChanged(value);
    }
}

QColor NDColorAttribute::getCurrentValue(void)
{
    return m_nValue;
}
