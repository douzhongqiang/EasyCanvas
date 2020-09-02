#include "NDAttributeBase.h"
#include "NDBoolAttribute.h"
#include "NDRealAttribute.h"
#include "NDColorAttribute.h"
#include "NDIntAttribute.h"
#include "NDStringAttribute.h"
#include "NDNodeBase.h"
#include "NDNodeManager.h"
#include "Utils.h"
#include <QColor>

NDAttributeBase::NDAttributeBase(NDNodeBase *parentNode)
    :QObject (parentNode)
{
    QObject::connect(this, &NDAttributeBase::valueChanged, \
                     this, &NDAttributeBase::onValueChaned);
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

QString NDAttributeBase::getFullName(void)
{
    if (m_pParentNode == nullptr)
        return "";

    return m_pParentNode->getNodeName() + "." + m_attributeName;
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

// 设置/获取父节点
void NDAttributeBase::setParentNode(NDNodeBase* node)
{
    m_pParentNode = node;
}

NDNodeBase* NDAttributeBase::getParentNode(void)
{
    return m_pParentNode;
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

QVariant NDAttributeBase::getLastValue(void)
{
    return m_lastValue;
}

QString NDAttributeBase::getTypeName(void)
{
    AttributeType type = this->Type();
    switch (type)
    {
    case t_bool:
        return "Bool";
    case t_int:
        return "Int";
    case t_qreal:
        return "QReal";
    case t_string:
        return "String";
    case t_stringList:
        return "StringList";
    case t_color:
        return "Color";
    case t_postion2d:
        return "Postion2D";
    }

    return "";
}

// 设置/获取属性值
void NDAttributeBase::setValue(const QVariant& value, bool cmd)
{
    // 处理第一次
    if (m_isFirstSetValue)
    {
        m_lastValue = value;
        m_isFirstSetValue = false;
    }

    // 判断是否需要发送信号
    bool needSendSignal = true;
    if (m_value == value)
        needSendSignal = false;

    QVariant tempValue = m_value;
    m_value = value;

    if (needSendSignal)
    {
        m_lastValue = tempValue;
        emit valueChanged(value, cmd);
    }
}

QVariant NDAttributeBase::getValue(void)
{
    return m_value;
}

void NDAttributeBase::onValueChaned(const QVariant& value, bool cmd)
{
    g_nodeManager->informAttributeValueChanged(this, value, cmd);
}
