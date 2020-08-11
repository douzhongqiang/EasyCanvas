#include "NDAttributeBase.h"
#include "NDBoolAttribute.h"
#include "NDRealAttribute.h"
#include "NDColorAttribute.h"
#include "NDIntAttribute.h"
#include "NDStringAttribute.h"
#include "NDNodeBase.h"
#include <QColor>

NDAttributeBase::NDAttributeBase(NDNodeBase *parentNode)
    :QObject (parentNode)
{

}

NDAttributeBase::~NDAttributeBase()
{

}

// 设置名字
void NDAttributeBase::setName(const QString& name)
{
    m_attributeName = name;
    if (m_displayName.isEmpty())
        m_displayName = name;
}

QString NDAttributeBase::getName(void)
{
    return m_attributeName;
}

// 设置显示名字
void NDAttributeBase::setDisplayName(const QString& name)
{
    m_displayName = name;
}

QString NDAttributeBase::getDisplayName(void)
{
    return m_displayName;
}

// 设置/获取父节点
void NDAttributeBase::setParentGroup(NDAttributeGroup* group)
{
    if (group == nullptr)
        return;

    this->setParent(group);
    m_pParentGroup = group;
}

NDAttributeGroup* NDAttributeBase::getParentGroup(void)
{
    return m_pParentGroup;
}

// 设置/获取使能
void NDAttributeBase::setEnable(bool isEnabled)
{
    if (isEnabled != m_isEnabled)
    {
        m_isEnabled = isEnabled;
        emit enabledChanged(m_isEnabled);
    }
}

bool NDAttributeBase::isEnable(void)
{
    return m_isEnabled;
}

NDAttributeBase* NDAttributeBase::createAttribute(const QString& name, AttributeType type, \
                                                  const QString& displayName)
{
    NDAttributeBase* attribute = nullptr;

    switch (type)
    {
    case t_bool:
    {
        attribute = new NDBoolAttribute;
        break;
    }
    case t_int:
    {
        attribute = new NDIntAttribute;
        break;
    }
    case t_qreal:
    {
        attribute = new NDRealAttribute;
        break;
    }
    case t_string:
    {
        attribute = new NDStringAttribute;
        break;
    }
    case t_color:
    {
        attribute = new NDColorAttribute;
        break;
    }
    default:
        break;
    }

    if (attribute == nullptr)
        return nullptr;

    // 设置属性名称
    attribute->setDisplayName(displayName);
    attribute->setName(name);
    return attribute;
}

QVariant NDAttributeBase::getCurrentValue(NDAttributeBase* pAttribute)
{
    AttributeType attrType = pAttribute->Type();

    switch (attrType)
    {
    case t_bool:
    {
        NDBoolAttribute* pValueAttr = dynamic_cast<NDBoolAttribute*>(pAttribute);
        return pValueAttr->getCurrentValue();
    }
    case t_int:
    {
        NDIntAttribute* pValueAttr = dynamic_cast<NDIntAttribute*>(pAttribute);
        return pValueAttr->getCurrentValue();
    }
    case t_qreal:
    {
        NDRealAttribute* pValueAttr = dynamic_cast<NDRealAttribute*>(pAttribute);
        return pValueAttr->getCurrentValue();
    }
    case t_string:
    {
        NDStringAttribute* pValueAttr = dynamic_cast<NDStringAttribute*>(pAttribute);
        return pValueAttr->getCurrentValue();
    }
    case t_color:
    {
        NDColorAttribute* pValueAttr = dynamic_cast<NDColorAttribute*>(pAttribute);
        return pValueAttr->getCurrentValue();
    }
    default:
        break;
    }

    return QVariant();
}
