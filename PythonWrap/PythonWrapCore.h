#ifndef PYTHONWRAPCORE_H
#define PYTHONWRAPCORE_H

#include <QObject>
#include <QWidget>
#include "pythonwrap_global.h"
#include "PythonQt.h"
#include "PythonQt_QtAll.h"

#define g_PythonWrapCode PythonWrapCore::getInstace()

class NDNodeBase;
class NDAttributeBase;
class PythonQtScriptingConsole;
class PYTHONWRAPSHARED_EXPORT PythonWrapCore : public QObject
{
    Q_OBJECT

public:
    static PythonWrapCore* getInstace(void);

    // 初始化
    void init(void);

    // 设置场景的节点
    void setSceneNode(NDNodeBase* sceneNode);

    // 显示脚本控制台
    QWidget* getScriptConsole(void);
    // 添加控制台输出
    void writeConsoleOutPut(const QString& message);

    // 执行文件
    void runScriptFiles(const QString& fileName);

public:
    // 下面是主要针对python的接口
    // 改变属性值
    Q_INVOKABLE void setAttribute(const QString& attributeName, const QVariant& var);
    // 添加和删除节点
    Q_INVOKABLE QString createNode(const QString& typeName);
    Q_INVOKABLE void deleteNode(const QString& nodeName);
    // 修改节点的名字
    Q_INVOKABLE void changeNodeName(const QString& srcName, const QString& destName);

    // 获取节点名等
    Q_INVOKABLE QStringList getAllNodes(void);
    // 获取选择的节点名
    Q_INVOKABLE QStringList getSelectedNodes(void);
    // 获取节点类型
    Q_INVOKABLE QString getNodeType(const QString& nodeName);
    // 获取属性列表
    Q_INVOKABLE QStringList getAttributeNames(const QString& nodeName);
    // 获取属性值
    Q_INVOKABLE QVariant getValue(const QString& attributeName);
    // 获取属性的类型
    Q_INVOKABLE QString getAttributeType(const QString& attributeName);

private:
    PythonWrapCore(QObject* parent = nullptr);
    ~PythonWrapCore();

    NDNodeBase* m_pSceneNode = nullptr;

    // 获取属性指针
    NDAttributeBase* getAttributeByName(NDNodeBase* node, const QString& attrName);
    // 为属性赋值
    void setAttributeValue(NDAttributeBase* pAttr, const QVariant& var);

private:
    PythonQtObjectPtr m_mainObject;

    PythonQtScriptingConsole* m_pScriptConsole = nullptr;
};

#endif
