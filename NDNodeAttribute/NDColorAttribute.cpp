#include "NDColorAttribute.h"
#include "NDNodeManager.h"
#include <QColor>

NDColorAttribute::NDColorAttribute(NDNodeBase *parentNode)
    :NDAttributeBase(parentNode)
{
    m_value.setValue<QColor>(QColor());
}

NDColorAttribute::~NDColorAttribute()
{

}
