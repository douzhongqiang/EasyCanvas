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

    // 获取上一次的属性值
    QVariant getLastValue(void);
    // 类型名转字符串
    QString getTypeName(void);

    // 设置/获取属性值
    virtual void setValue(const QVariant& value, bool cmd = false);
    virtual QVariant getValue(void);

    // 设置名字
    void setName(const QString& name);
    QString getName(void);

    // 设置显示名字
    void setDisplayName(const QString& name);
    QString getDisplayName(void);

    // 获取全名
    QString getFullName(void);

    // 设置/获取父节点组
    void setParentGroup(NDAttributeGroup* group);
    NDAttributeGroup* getParentGroup(void);

    // 设置/获取父节点
    void setParentNode(NDNodeBase* node);
    NDNodeBase* getParentNode(void);

    // 设置/获取使能
    void setEnable(bool isEnabled);
    bool isEnable(void);

protected:
    AttributeType m_attributeType;
    QString m_attributeName;
    QString m_displayName;

    bool m_isEnabled = true;
    QVariant m_lastValue;
    QVariant m_value;

    bool m_isFirstSetValue = true;

    NDAttributeGroup* m_pParentGroup = nullptr;
    NDNodeBase* m_pParentNode = nullptr;

signals:
    void enabledChanged(bool enabled);
    void valueChanged(const QVariant& value, bool cmd = false);

private slots:
    void onValueChaned(const QVariant& value, bool cmd);
};

#endif
