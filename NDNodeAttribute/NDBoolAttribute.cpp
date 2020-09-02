#include "NDBoolAttribute.h"
#include "NDNodeManager.h"

NDBoolAttribute::NDBoolAttribute(NDNodeBase *parentNode)
    :NDAttributeBase(parentNode)
{
    m_value = true;
}

NDBoolAttribute::~NDBoolAttribute()
{

}
