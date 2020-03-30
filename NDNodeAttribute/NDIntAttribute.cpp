#include "NDIntAttribute.h"

NDIntAttribute::NDIntAttribute(NDNodeBase* parentNode)
    :NDAttributeBase(parentNode)
{

}

NDIntAttribute::~NDIntAttribute()
{

}

// 设置/获取 当前值
void NDIntAttribute::setCurrentValue(int value)
{
    bool needSendSignal = true;
    if (m_nValue == value)
        needSendSignal = false;

    m_nValue = value;

    // 发送信号
    if (needSendSignal)
        emit valueChanged(value);
}

int NDIntAttribute::getCurrentValue(void)
{
    return m_nValue;
}

// 设置/获取 取值范围
void NDIntAttribute::setValueRange(int minValue, int maxValue)
{
    m_nMinValue = minValue;
    m_nMaxValue = maxValue;
}

void NDIntAttribute::getValueRange(int& minValue, int& maxValue)
{
    minValue = m_nMinValue;
    maxValue = m_nMaxValue;
}
