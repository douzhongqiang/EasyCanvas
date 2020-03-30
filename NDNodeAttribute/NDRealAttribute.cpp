#include "NDRealAttribute.h"
#include "Utils.h"

NDRealAttribute::NDRealAttribute(NDNodeBase* parentNode)
    :NDAttributeBase(parentNode)
{

}

NDRealAttribute::~NDRealAttribute()
{

}

// 设置/获取 当前值
void NDRealAttribute::setCurrentValue(qreal value)
{
    bool needSendSignal = true;
    if (g_utilTool->isEqual(m_qValue, value))
        needSendSignal = false;

    m_qValue = value;

    if (needSendSignal)
        emit valueChanged(value);
}

qreal NDRealAttribute::getCurrentValue(void)
{
    return m_qValue;
}

// 设置/获取 取值范围
void NDRealAttribute::setValueRange(qreal minValue, qreal maxValue)
{
    m_qMinValue = minValue;
    m_qMaxValue = maxValue;
}

void NDRealAttribute::getValueRange(qreal& minValue, qreal& maxValue)
{
    minValue = m_qMinValue;
    maxValue = m_qMaxValue;
}
