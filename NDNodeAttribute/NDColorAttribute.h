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
};

#endif
