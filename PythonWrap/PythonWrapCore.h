#ifndef PYTHONWRAPCORE_H
#define PYTHONWRAPCORE_H

#include <QObject>
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
    void showScriptingConsole(void);

public:
    // 下面是主要针对python的接口
    // 改变属性值
    Q_INVOKABLE void setAttribute(const QString& attributeName, const QVariant& var);

    Q_INVOKABLE void createNode(const QString& typeName);
    Q_INVOKABLE void deleteNode(const QString& nodeName);

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
