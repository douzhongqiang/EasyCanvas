#ifndef NDNODEMANAGER_H
#define NDNODEMANAGER_H

#include <QObject>
#include <QList>
#include <QString>

#include "NDNodeBase.h"
#include "ndnodeattribute_global.h"
#define g_nodeManager NDNodeManager::getInstance()
class NDNODEATTRIBUTESHARED_EXPORT NDNodeManager : public QObject
{
    Q_OBJECT

public:
    NDNodeManager(QObject* parent = nullptr)  ;
    ~NDNodeManager();

    static NDNodeManager* getInstance(void);

    // 添加节点（目前不支持节点中添加子节点）
    void addNode(NDNodeBase* node);
    void addNode(NDNodeBase::NodeType type);

    // 删除节点
    void deleteNode(NDNodeBase* node);
    void deleteNode(const QString& nodeName);

    // 设置当前选中的节点
    void setCurrentSelectedNode(NDNodeBase* node);

    // 创建一个节点
    NDNodeBase* createNode(NDNodeBase::NodeType nodeType);

    // 获取节点列表
    void getNodeList(QList<NDNodeBase*>& nodelist);

private:
    QList<NDNodeBase*> nodeList;            // 节点列表
    QList<NDNodeBase*> deletedNodeList;     // 删除的节点列表
    int* m_pStartCountPtr = nullptr;       // 计数

    NDNodeBase* m_selectedNode = nullptr;

    // 创建节点名字
    QString createNodeName(int type);

signals:
    void nodeAdded(NDNodeBase* node);
    void nodeDeleted(NDNodeBase* node);
    void nodeSelectChanged(NDNodeBase* node);
};

#endif
