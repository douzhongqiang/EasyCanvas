#ifndef NDBOOLATTRIBTE_H
#define NDBOOLATTRIBTE_H

#include "NDAttributeBase.h"
#include "ndnodeattribute_global.h"

class NDNODEATTRIBUTESHARED_EXPORT NDBoolAttribute : public NDAttributeBase
{
    Q_OBJECT
public:
    NDBoolAttribute(NDNodeBase *parentNode = nullptr);
    ~NDBoolAttribute();

    AttributeType Type(void) const override{
        return t_bool;
    }

    // 设置/获取当前值
    void setCurrentValue(bool value);
    bool getCurrentValue(void);

private:
    bool m_value = true;

signals:
    void valueChanged(bool value);
};

#endif
