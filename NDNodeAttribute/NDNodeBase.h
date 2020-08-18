#ifndef NDNODEBASE_H
#define NDNODEBASE_H

#include <QObject>
#include "NDAttributeBase.h"
#include "NDAttributeGroup.h"
#include "ndnodeattribute_global.h"
class NDNODEATTRIBUTESHARED_EXPORT NDNodeBase : public QObject
{
    Q_OBJECT

public:
    enum NodeType
    {
        t_textNode,
        t_RectNode,
        t_Elleipse,
        t_Arrow,
        t_FreeDraw,

        t_User,         // 自定义节点类型
        t_End
    };

public:
    NDNodeBase(QObject* parent = nullptr);
    NDNodeBase(NodeType type, QObject* parent = nullptr);
    ~NDNodeBase();

    // 设置/获取节点类型
    void setNodeType(int type);
    virtual int getNodeType(void);

    // 设置节点名字
    void setNodeName(const QString& nodeName);
    QString getNodeName(void);

    // 获取属性组
    void getAllAttributeGroups(QList<NDAttributeGroup*>& groups);

    // 添加属性组
    void addAttributeGroup(NDAttributeGroup* group);
    NDAttributeGroup* addAttributeGroup(const QString& name);
    NDAttributeGroup* addAttributeGroup(const QString& name, const QString& displayName);
    // 删除属性组
    void deleteAttributeGroup(NDAttributeGroup* group);
    void deleteAttributeGroup(const QString& name);

    // 添加属性
    bool addAttribute(NDAttributeGroup* group, NDAttributeBase* attribute);
    bool addAttribute(const QString& groupName, NDAttributeBase* attribute);
    bool addAttribute(const QString& groupName, const QString& name, NDAttributeBase::AttributeType type);
    // 删除属性
    bool deleteAttribute(NDAttributeBase* attribute);
    bool deleteAttribute(const QString& name);

    // 查找属性
    NDAttributeBase* getAttribute(const QString& attrName);

private:
    QList<NDAttributeGroup*> m_groupList;
    int m_nodeType;

    QString m_name;     // 节点名字
};

#endif
