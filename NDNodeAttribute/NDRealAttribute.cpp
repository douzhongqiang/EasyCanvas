#include "NDRealAttribute.h"
#include "Utils.h"
#include "NDNodeManager.h"

NDRealAttribute::NDRealAttribute(NDNodeBase* parentNode)
    :NDAttributeBase(parentNode)
{
    m_value = 0;
}

NDRealAttribute::~NDRealAttribute()
{

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
