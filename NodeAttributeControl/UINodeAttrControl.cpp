#include "UINodeAttrControl.h"
#include "UIAttrColorControl.h"
#include "UIAttrTextControl.h"
#include "UIAttrFloatControl.h"
#include "UIAttrIntControl.h"
#include "UIAttrBoolControl.h"
#include "NDAttributeBase.h"
#include "NDAttributeGroup.h"
#include "CustomCombineControl/UICustomGroupControl.h"
#include <QVBoxLayout>

UINodeAttrControl::UINodeAttrControl(QObject* parent)
    :QObject(parent)
{

}

UINodeAttrControl::~UINodeAttrControl()
{

}

QWidget* UINodeAttrControl::createNodeWidget(NDNodeBase* node)
{
    if (node == nullptr)
        return nullptr;

    // 获取所有的属性组
    QList<NDAttributeGroup*> attributeGroupList;
    node->getAllAttributeGroups(attributeGroupList);

    if (attributeGroupList.count() <= 0)
        return nullptr;

    QWidget* pWidget = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout(pWidget);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(8);

    // 添加属性组
    foreach(auto group, attributeGroupList)
    {
        QWidget* groupControl = createAttributeGroupControl(group);
        mainLayout->addWidget(groupControl, 0, Qt::AlignTop);
    }
    mainLayout->addStretch();

    return pWidget;
}

QWidget* UINodeAttrControl::createAttributeControl(NDAttributeBase* attribute)
{
    if (attribute == nullptr)
        return nullptr;

    NDAttributeBase::AttributeType attrType = attribute->Type();
    switch (attrType)
    {
    case NDAttributeBase::t_color:
    {
        return new UIAttrColorControl(attribute);
    }
    case NDAttributeBase::t_string:
    {
        return new UIAttrTextControl(attribute);
    }
    case NDAttributeBase::t_qreal:
    {
        return new UIAttrFloatControl(attribute);
    }
    case NDAttributeBase::t_int:
    {
        return new UIAttrIntControl(attribute);
    }
    case NDAttributeBase::t_bool:
    {
        return new UIAttrBoolControl(attribute);
    }
    default:
        break;
    }

    return nullptr;
}

QWidget* UINodeAttrControl::createAttributeGroupControl(NDAttributeGroup* group)
{
    if (group == nullptr)
        return nullptr;

    // 添加group control
    UICustomGroupControl* groupControl = new UICustomGroupControl;
    groupControl->setTitleText(group->getDisplayName());

    // 添加属性控件
    QList<NDAttributeBase*> attributes;
    group->getAttributes(attributes);
    foreach(auto attribute, attributes)
    {
        QWidget* pWidget = createAttributeControl(attribute);
        if (pWidget == nullptr)
            continue;

        groupControl->addContentWidget(pWidget);
    }

    return groupControl;
}
