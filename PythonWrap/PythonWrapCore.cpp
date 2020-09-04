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
#include "UICanvas/UICanvasItemBase.h"
#include <QSharedPointer>
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
    qDebug() << "PythonWrap Init";

    // init PythonQt and Python
    PythonQt::init();
    PythonQt_QtAll::init();

    m_mainObject = PythonQt::self()->getMainModule();
    if (!m_mainObject.isNull())
        m_mainObject.addObject("easyCanvas", g_PythonWrapCode);
    qDebug() << "PythonWrap End";
}

// 改变属性值
void PythonWrapCore::setAttribute(const QString& attributeName, const QVariant& var)
{
    // 获取节点名称
    int index = attributeName.indexOf(".");
    if (index < 0)
    {
        writeConsoleOutPut("Attribute Error");
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
            writeConsoleOutPut("Attribute Error");
            return;
        }

        // 设置值
        setAttributeValue(pAttr, var);
        return;
    }

    // 处理普通元素节点
    NDNodeBase* pNode = g_currentCanvasManager->getNode(nodeName);
    if (pNode == nullptr)
    {
        writeConsoleOutPut(QString("Not Has This Node ") + nodeName);
        return;
    }
    // 获取属性
    NDAttributeBase* pAttr = getAttributeByName(pNode, attrName);
    if (pAttr == nullptr)
    {
        writeConsoleOutPut(QString("Not Has This Attribute ") + attrName);
        return;
    }
    setAttributeValue(pAttr, var);
}

QString PythonWrapCore::createNode(const QString& typeName)
{
    UICanvasItemManager::CanvasItemType type = g_currentCanvasManager->getTypeByName(typeName);
    if (type == UICanvasItemManager::t_None || type == UICanvasItemManager::t_CanvasItem)
    {
        writeConsoleOutPut("Can't Create This Type Node!");
        return "";
    }
    QSharedPointer<UICanvasItemBase> node = g_currentCanvasManager->createCanvasItemByCmd(type);
    if (node.isNull())
    {
        writeConsoleOutPut("Create Node Error!");
        return "";
    }
    return node->getCurrentNode()->getNodeName();
}

void PythonWrapCore::deleteNode(const QString& nodeName)
{
    if (g_currentCanvasManager->getNode(nodeName) == nullptr)
    {
        writeConsoleOutPut(QString("Not Has This Node ") + nodeName);
        return;
    }

    QStringList strs;
    strs << nodeName;
    g_currentCanvasManager->deleteCanvasItemByCmd(strs);
}

void PythonWrapCore::changeNodeName(const QString& srcName, const QString& destName)
{
    if (!g_currentCanvasManager->isCanChangedName(srcName, destName))
    {
        writeConsoleOutPut("Can't Changed Node Name!");
        return;
    }

    g_currentCanvasManager->changedNodeNameCmd(srcName, destName);
}

QStringList PythonWrapCore::getAllNodes(void)
{
    return g_currentCanvasManager->getAllNodeNames();
}

QStringList PythonWrapCore::getSelectedNodes(void)
{
    return g_currentCanvasManager->getSelectedNodes();
}

QString PythonWrapCore::getNodeType(const QString& nodeName)
{
    NDNodeBase* pNode = g_currentCanvasManager->getNode(nodeName);
    if (pNode == nullptr)
    {
        writeConsoleOutPut("Can't Finde This Node!");
        return "";
    }

    return g_currentCanvasManager->getTypeName((UICanvasItemManager::CanvasItemType)pNode->getNodeType());
}

QStringList PythonWrapCore::getAttributeNames(const QString& nodeName)
{
    QStringList strs;
    NDNodeBase* pNode = g_currentCanvasManager->getNode(nodeName);
    if (pNode == nullptr)
    {
        writeConsoleOutPut("Can't Finde This Node!");
        return strs;
    }

    QList<NDAttributeGroup*> groups;
    pNode->getAllAttributeGroups(groups);
    for (auto iter = groups.begin(); iter != groups.end(); ++iter)
    {
        QList<NDAttributeBase*> attrs;
        (*iter)->getAttributes(attrs);

        for (auto nIter = attrs.begin(); nIter != attrs.end(); ++nIter)
        {
            strs << (*nIter)->getName();
        }
    }
    return strs;
}

Q_INVOKABLE QVariant PythonWrapCore::getValue(const QString& attributeName)
{
    // 获取节点名称
    int index = attributeName.indexOf(".");
    if (index < 0)
    {
        writeConsoleOutPut("Attribute Error");
        return "";
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
            writeConsoleOutPut("Attribute Error");
            return "";
        }

        // 设置值
        return pAttr->getValue();
    }

    // 处理普通元素节点
    NDNodeBase* pNode = g_currentCanvasManager->getNode(nodeName);
    if (pNode == nullptr)
    {
        writeConsoleOutPut(QString("Not Has This Node ") + nodeName);
        return "";
    }
    // 获取属性
    NDAttributeBase* pAttr = getAttributeByName(pNode, attrName);
    if (pAttr == nullptr)
    {
        writeConsoleOutPut(QString("Not Has This Attribute ") + attrName);
        return "";
    }
    return pAttr->getValue();
}

QString PythonWrapCore::getAttributeType(const QString& attributeName)
{
    // 获取节点名称
    int index = attributeName.indexOf(".");
    if (index < 0)
    {
        writeConsoleOutPut("Attribute Error");
        return "";
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
            writeConsoleOutPut("Attribute Error");
            return "";
        }

        // 设置值
        return pAttr->getTypeName();
    }

    // 处理普通元素节点
    NDNodeBase* pNode = g_currentCanvasManager->getNode(nodeName);
    if (pNode == nullptr)
    {
        writeConsoleOutPut(QString("Not Has This Node ") + nodeName);
        return "";
    }
    // 获取属性
    NDAttributeBase* pAttr = getAttributeByName(pNode, attrName);
    if (pAttr == nullptr)
    {
        writeConsoleOutPut(QString("Not Has This Attribute ") + attrName);
        return "";
    }
    return pAttr->getTypeName();
}

void PythonWrapCore::setSceneNode(NDNodeBase* sceneNode)
{
    m_pSceneNode = sceneNode;
}

QWidget* PythonWrapCore::getScriptConsole(void)
{
    if (m_pScriptConsole == nullptr)
    {
        m_pScriptConsole = new PythonQtScriptingConsole(nullptr, m_mainObject);
        m_pScriptConsole->resize(800, 600);
    }

    return m_pScriptConsole;
}

void PythonWrapCore::writeConsoleOutPut(const QString& message)
{
    m_pScriptConsole->stdOut(message);
}

void PythonWrapCore::runScriptFiles(const QString& fileName)
{
    if (!QFile::exists(fileName))
    {
        writeConsoleOutPut("The Script File is not Existed!");
        return;
    }

    QFile file(fileName);
    file.open(QFile::ReadOnly);
    QString pyFile = file.readAll();
    file.close();

    m_mainObject.evalScript(pyFile);
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

    pAttr->setValue(var, true);
}
