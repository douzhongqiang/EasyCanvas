#ifndef NDATTRIBUTEBASE_H
#define NDATTRIBUTEBASE_H

#include <QObject>
#include <QVariant>
#include "ndnodeattribute_global.h"
class NDNodeBase;
class NDAttributeGroup;
class NDNODEATTRIBUTESHARED_EXPORT NDAttributeBase : public QObject
{
    Q_OBJECT

public:
    enum AttributeType
    {
        t_bool,             // 布尔类型
        t_int,              // int 型
        t_qreal,            // 浮点型
        t_string,           // 字符串类型
        t_stringList,       // 字符串数组
        t_color,            // 颜色
        t_postion2d         // 2D坐标类型
    };

public:
    NDAttributeBase(NDNodeBase *parentNode = nullptr);
    virtual ~NDAttributeBase();

    virtual AttributeType Type(void) const = 0 ;

    static NDAttributeBase* createAttribute(const QString& name, AttributeType type, const QString& displayName = "");
    // 设置/获取属性
    static QVariant getCurrentValue(NDAttributeBase* pAttribute);
    static void setCurrentValue(NDAttributeBase* pAttribute, const QVariant value);

    // 设置名字
    void setName(const QString& name);
    QString getName(void);

    // 设置显示名字
    void setDisplayName(const QString& name);
    QString getDisplayName(void);

    // 设置/获取父节点
    void setParentGroup(NDAttributeGroup* group);
    NDAttributeGroup* getParentGroup(void);

    // 设置/获取使能
    void setEnable(bool isEnabled);
    bool isEnable(void);

private:
    AttributeType m_attributeType;
    QString m_attributeName;
    QString m_displayName;

    bool m_isEnabled = true;

    NDAttributeGroup* m_pParentGroup = nullptr;

signals:
    void enabledChanged(bool enabled);
};

#endif
