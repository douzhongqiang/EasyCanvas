#ifndef NDATTRIBUTEGROUP_H
#define NDATTRIBUTEGROUP_H

#include <QObject>
#include "ndnodeattribute_global.h"

class NDAttributeBase;
class NDNodeBase;
class NDNODEATTRIBUTESHARED_EXPORT NDAttributeGroup : public QObject
{
    Q_OBJECT

public:
    NDAttributeGroup(QObject* parent = nullptr);
    ~NDAttributeGroup();

    // 添加删除属性
    void addAttribute(NDAttributeBase* attribute);
    // 删除属性
    void deleteAttribute(NDAttributeBase* attribute);
    // 获取全部属性
    void getAttributes(QList<NDAttributeBase*>& attributes);
    NDAttributeBase* getAttribute(const QString& name);

    // 设置父节点
    void setParentNode(NDNodeBase* node);
    NDNodeBase* getParentNode(void);

    // 设置/获取属性组的名称
    void setName(const QString& name);
    QString getName(void);

    // 设置/获取属性组的显示名称
    void setDisplayName(const QString& name);
    QString getDisplayName(void);

private:
    NDNodeBase* m_pParentNode = nullptr;
    QList<NDAttributeBase*> m_attributes;
    QString m_groupName;
    QString m_displayName;
};

#endif
