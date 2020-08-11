#include "NDAttributeGroup.h"
#include "NDAttributeBase.h"
#include "NDNodeBase.h"

NDAttributeGroup::NDAttributeGroup(QObject* parent)
    :QObject(parent)
{

}

NDAttributeGroup::~NDAttributeGroup()
{

}

// 添加删除属性
void NDAttributeGroup::addAttribute(NDAttributeBase* attribute)
{
    if (attribute == nullptr)
        return;

    m_attributes.push_back(attribute);
    attribute->setParentGroup(this);
}

// 删除属性
void NDAttributeGroup::deleteAttribute(NDAttributeBase* attribute)
{
    for (auto iter = m_attributes.begin(); iter != m_attributes.end(); ++iter)
    {
        if (*iter == attribute)
        {
            m_attributes.erase(iter);
            attribute->deleteLater();
            break;
        }
    }
}

void NDAttributeGroup::getAttributes(QList<NDAttributeBase*>& attributes)
{
    attributes = m_attributes;
}

NDAttributeBase* NDAttributeGroup::getAttribute(const QString& name)
{
    foreach (auto item, m_attributes)
        if (item->getName() == name)
            return item;

    return nullptr;
}

// 设置父节点
void NDAttributeGroup::setParentNode(NDNodeBase* node)
{
    m_pParentNode = node;
    this->setParent(node);
}

NDNodeBase* NDAttributeGroup::getParentNode(void)
{
    return m_pParentNode;
}

// 设置/获取属性组的名称
void NDAttributeGroup::setName(const QString& name)
{
    m_groupName = name;
}

QString NDAttributeGroup::getName(void)
{
    return m_groupName;
}

void NDAttributeGroup::setDisplayName(const QString& name)
{
    m_displayName = name;
}

QString NDAttributeGroup::getDisplayName(void)
{
    return m_displayName;
}
