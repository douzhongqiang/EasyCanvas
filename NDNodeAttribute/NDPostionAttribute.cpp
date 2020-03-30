#include "NDPostionAttribute.h"

NDPostionAttribute::NDPostionAttribute(NDNodeBase *parentNode)
    :NDAttributeBase(parentNode)
{

}

NDPostionAttribute::~NDPostionAttribute()
{

}

// 设置/获取 当前值
void NDPostionAttribute::setCurrentValue(const QPoint& value)
{
    bool needSendSignal = true;
    if (m_value == value)
        needSendSignal = false;

    m_value = value;

    // 发送信号
    if (needSendSignal)
        emit valueChanged(value);
}

QPoint NDPostionAttribute::getCurrentValue(void)
{
    return m_value;
}
