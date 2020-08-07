#include "PythonWrapCore.h"
#include "NDNodeBase.h"
#include "NDAttributeGroup.h"
#include "NDAttributeBase.h"
#include "NDBoolAttribute.h"
#include "NDColorAttribute.h"
#include "NDIntAttribute.h"
#include "NDRealAttribute.h"
#include "NDPostionAttribute.h"
#include "NDRealAttribute.h"
#include "NDStringAttribute.h"
#include "gui/PythonQtScriptingConsole.h"
#include "UICanvas/UICanvasItemManager.h"
#include <QDebug>
#include <QVariant>

PythonWrapCore::PythonWrapCore(QObject* parent)
    :QObject(parent)
{

}

PythonWrapCore::~PythonWrapCore()
{

}

PythonWrapCore* PythonWrapCore::getInstace(void)
{
    static PythonWrapCore instance;
    return &instance;
}

void PythonWrapCore::init(void)
{
    // init PythonQt and Python
    PythonQt::init(PythonQt::IgnoreSiteModule | PythonQt::RedirectStdOut);
    PythonQt_QtAll::init();

    m_mainObject = PythonQt::self()->getMainModule();
    m_mainObject.addObject("easyCanvas", g_PythonWrapCode);
}

// 改变属性值
void PythonWrapCore::setAttribute(const QString& attributeName, const QVariant& var)
{
    // 获取节点名称
    int index = attributeName.indexOf(".");
    if (index < 0)
    {
        qDebug() << "Attribute Error";
        return;
    }
    QString nodeName = attributeName.left(index);
    QString attrName = attributeName.right(attributeName.length() - index - 1);

    // 处理场景节点
    if (m_pSceneNode->getNodeName() == nodeName)
    {
        // 获取属性指针
        NDAttributeBase* pAttr = getAttributeByName(m_pSceneNode, attrName);
        if (pAttr == nullptr)
        {
            qDebug() << "Attribute Error";
            return;
        }

        // 设置值
        setAttributeValue(pAttr, var);
        return;
    }

    // 处理普通元素节点
    NDNodeBase* pNode = g_currentCanvasManager->getNode(nodeName);
    if (pNode == nullptr)
        return;
    // 获取属性
    NDAttributeBase* pAttr = getAttributeByName(pNode, attrName);
    if (pAttr == nullptr)
        return;
    setAttributeValue(pAttr, var);
}

void PythonWrapCore::createNode(const QString& typeName)
{

}

void PythonWrapCore::deleteNode(const QString& nodeName)
{
    g_currentCanvasManager->deleteCanvasItem(nodeName);
}

void PythonWrapCore::setSceneNode(NDNodeBase* sceneNode)
{
    m_pSceneNode = sceneNode;
}

void PythonWrapCore::showScriptingConsole(void)
{
    if (m_pScriptConsole == nullptr)
    {
        m_pScriptConsole = new PythonQtScriptingConsole(nullptr, m_mainObject);
        m_pScriptConsole->resize(800, 600);
    }

    m_pScriptConsole->show();
    m_pScriptConsole->raise();
}

NDAttributeBase* PythonWrapCore::getAttributeByName(NDNodeBase* node, const QString& attrName)
{
    if (node == nullptr)
        return nullptr;

    // 获取属性组
    QList<NDAttributeGroup*> attrGroups;
    node->getAllAttributeGroups(attrGroups);

    for (auto iter = attrGroups.begin(); iter != attrGroups.end(); ++iter)
    {
        NDAttributeBase* pAttr = (*iter)->getAttribute(attrName);
        if (pAttr != nullptr)
            return pAttr;
    }

    return nullptr;
}

void PythonWrapCore::setAttributeValue(NDAttributeBase* pAttr, const QVariant& var)
{
    if (pAttr == nullptr)
        return;

    switch (pAttr->Type())
    {
    case NDAttributeBase::t_bool:
    {
        NDBoolAttribute* pBoolAttr = qobject_cast<NDBoolAttribute*>(pAttr);
        if (pBoolAttr)
            pBoolAttr->setCurrentValue(var.toBool());
        return;
    }
    case NDAttributeBase::t_int:
    {
        NDIntAttribute* pIntAttr = qobject_cast<NDIntAttribute*>(pAttr);
        if (pIntAttr)
            pIntAttr->setCurrentValue(var.toInt());
        return;
    }
    case NDAttributeBase::t_color:
    {
        NDColorAttribute* pColorAttr = qobject_cast<NDColorAttribute*>(pAttr);
        if (pColorAttr)
        {
            QColor color = var.value<QColor>();
            pColorAttr->setCurrentValue(color);
        }
        return;
    }
    case NDAttributeBase::t_qreal:
    {
        NDRealAttribute* pRealAttr = qobject_cast<NDRealAttribute*>(pAttr);
        if (pRealAttr)
            pRealAttr->setCurrentValue(var.toReal());
        return;
    }
    case NDAttributeBase::t_string:
    {
        NDStringAttribute* pStringAttr = qobject_cast<NDStringAttribute*>(pAttr);
        if (pStringAttr)
            pStringAttr->setCurrentValue(var.toString());
        return;
    }
    }
}
