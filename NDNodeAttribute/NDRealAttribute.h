#ifndef NDREALATTRIBUTE_H
#define NDREALATTRIBUTE_H

#include "NDAttributeBase.h"
#include "ndnodeattribute_global.h"
class NDNodeBase;
class NDNODEATTRIBUTESHARED_EXPORT NDRealAttribute : public NDAttributeBase
{
    Q_OBJECT

public:
    NDRealAttribute(NDNodeBase* parentNode = nullptr);
    ~NDRealAttribute();

    AttributeType Type(void) const override{
        return t_qreal;
    }

    // 设置/获取 取值范围
    void setValueRange(qreal minValue, qreal maxValue);
    void getValueRange(qreal& minValue, qreal& maxValue);

private:
    qreal m_qMinValue = 0;
    qreal m_qMaxValue = 100;
};

#endif
