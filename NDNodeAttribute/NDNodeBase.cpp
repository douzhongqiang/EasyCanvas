#include "NDNodeBase.h"

NDNodeBase::NDNodeBase(NodeType type, QObject* parent)
    :QObject(parent)
    ,m_nodeType(type)
{

}

NDNodeBase::NDNodeBase(QObject* parent)
    :QObject (parent)
{

}

NDNodeBase::~NDNodeBase()
{

}

// 设置/获取节点类型
void NDNodeBase::setNodeType(int type)
{
    m_nodeType = type;
}

int NDNodeBase::getNodeType(void)
{
    return m_nodeType;
}

// 添加属性
bool NDNodeBase::addAttribute(NDAttributeGroup* group, NDAttributeBase* attribute)
{
    if (group == nullptr || attribute == nullptr)
        return false;

    group->addAttribute(attribute);
    attribute->setParentNode(this);
    return true;
}

bool NDNodeBase::addAttribute(const QString& groupName, NDAttributeBase* attribute)
{
    if (attribute == nullptr)
        return false;

    NDAttributeGroup* attributeGroup = nullptr;
    foreach(auto item, m_groupList)
    {
        if (groupName == item->getName())
        {
            attributeGroup = item;
            break;
        }
    }

    if (attributeGroup == nullptr)
        return false;

    return addAttribute(attributeGroup, attribute);
}

bool NDNodeBase::addAttribute(const QString& groupName, const QString& name, NDAttributeBase::AttributeType type)
{
    NDAttributeBase* attribute = NDAttributeBase::createAttribute(name, type);
    addAttribute(groupName, attribute);
    return true;
}

// 删除属性
bool NDNodeBase::deleteAttribute(NDAttributeBase* attribute)
{
    auto group = attribute->getParentGroup();
    if (group == nullptr)
        return false;

    group->deleteAttribute(attribute);
    return true;
}

bool NDNodeBase::deleteAttribute(const QString& name)
{
    foreach (auto item, m_groupList)
    {
        if (NDAttributeBase* attribute = item->getAttribute(name))
        {
            deleteAttribute(attribute);
            return true;
        }
    }

    return false;
}


// 查找属性
NDAttributeBase* NDNodeBase::getAttribute(const QString& attrName)
{
    foreach (auto item, m_groupList)
    {
        if (NDAttributeBase* attribute = item->getAttribute(attrName))
        {
            return attribute;
        }
    }

    return nullptr;
}

// 设置节点名字
void NDNodeBase::setNodeName(const QString& nodeName)
{
    m_name = nodeName;
}

QString NDNodeBase::getNodeName(void)
{
    return m_name;
}

void NDNodeBase::getAllAttributeGroups(QList<NDAttributeGroup*>& groups)
{
    groups = m_groupList;
}

void NDNodeBase::addAttributeGroup(NDAttributeGroup* group)
{
    if (group == nullptr)
        return;

    group->setParentNode(this);
    m_groupList.append(group);
}

NDAttributeGroup* NDNodeBase::addAttributeGroup(const QString& name)
{
    NDAttributeGroup* group = new NDAttributeGroup;
    group->setName(name);
    group->setDisplayName(name);
    addAttributeGroup(group);

    return group;
}

NDAttributeGroup* NDNodeBase::addAttributeGroup(const QString& name, const QString& displayName)
{
    NDAttributeGroup* group = new NDAttributeGroup;
    group->setName(name);
    group->setDisplayName(displayName);
    addAttributeGroup(group);

    return group;
}

// 删除属性组
void NDNodeBase::deleteAttributeGroup(NDAttributeGroup* group)
{
    for (auto iter = m_groupList.begin(); iter != m_groupList.end(); ++iter)
    {
        if (*iter == group)
        {
            m_groupList.erase(iter);
            group->deleteLater();
            break;
        }
    }
}

void NDNodeBase::deleteAttributeGroup(const QString& name)
{
    for (auto iter = m_groupList.begin(); iter != m_groupList.end(); ++iter)
    {

        if ((*iter)->getName() == name)
        {
            m_groupList.erase(iter);
            (*iter)->deleteLater();

            break;
        }
    }
}
