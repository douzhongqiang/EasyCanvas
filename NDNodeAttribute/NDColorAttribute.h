#ifndef NDCOLORATTRIBUTE_H
#define NDCOLORATTRIBUTE_H

#include "NDAttributeBase.h"
#include "ndnodeattribute_global.h"
#include <QColor>
class NDNODEATTRIBUTESHARED_EXPORT NDColorAttribute : public NDAttributeBase
{
    Q_OBJECT

public:
    NDColorAttribute(NDNodeBase *parentNode = nullptr);
    ~NDColorAttribute();

    AttributeType Type(void) const override{
        return t_color;
    }

    // 设置/获取当前颜色
    void setCurrentValue(const QColor& value);
    QColor getCurrentValue(void);

private:
    QColor m_nValue;

signals:
    void valueChanged(const QColor& value);
};

#endif
