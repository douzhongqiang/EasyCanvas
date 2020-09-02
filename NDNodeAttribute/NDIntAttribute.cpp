#include "NDIntAttribute.h"
#include "NDNodeManager.h"

NDIntAttribute::NDIntAttribute(NDNodeBase* parentNode)
    :NDAttributeBase(parentNode)
{
    m_value = 0;
}

NDIntAttribute::~NDIntAttribute()
{

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
