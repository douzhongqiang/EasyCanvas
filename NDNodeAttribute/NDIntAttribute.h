#ifndef NDINTATTRIBUTE_H
#define NDINTATTRIBUTE_H

#include "NDAttributeBase.h"
#include "ndnodeattribute_global.h"
class NDNODEATTRIBUTESHARED_EXPORT NDIntAttribute : public NDAttributeBase
{
    Q_OBJECT

public:
    NDIntAttribute(NDNodeBase* parentNode = nullptr);
    ~NDIntAttribute();

    AttributeType Type(void) const{
        return t_int;
    }

    // 设置/获取 取值范围
    void setValueRange(int minValue, int maxValue);
    void getValueRange(int& minValue, int& maxValue);

private:
    int m_nMinValue = 0;
    int m_nMaxValue = 100;
};

#endif
