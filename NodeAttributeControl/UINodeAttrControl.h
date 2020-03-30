#ifndef UINODEATTRCONTROL_H
#define UINODEATTRCONTROL_H

#include "NDNodeManager.h"
#include "NDNodeBase.h"
#include "nodeattributecontrol_global.h"

#include <QObject>
#include <QWidget>
class NDAttributeBase;
class NDAttributeGroup;
class NODEATTRIBUTECONTROLSHARED_EXPORT UINodeAttrControl : public QObject
{
    Q_OBJECT

public:
    UINodeAttrControl(QObject* parent = nullptr);
    ~UINodeAttrControl();

    // 根据节点创建Widget
    static QWidget* createNodeWidget(NDNodeBase* node);

private:
    // 创建属性控件
    static QWidget* createAttributeControl(NDAttributeBase* attribute);
    // 创建属性组Widget
    static QWidget* createAttributeGroupControl(NDAttributeGroup* group);
};

#endif
