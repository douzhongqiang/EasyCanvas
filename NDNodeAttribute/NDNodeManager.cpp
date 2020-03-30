#include "NDNodeManager.h"
#include "NDTextNode.h"

NDNodeManager::NDNodeManager(QObject* parent)
    :QObject(parent)
{

    int count = (int)NDNodeBase::t_End;
    m_pStartCountPtr = new int[count];
    memset(m_pStartCountPtr, 0, sizeof(int) * count);
}

NDNodeManager::~NDNodeManager()
{
    delete [] m_pStartCountPtr;

    // 删除节点
    for (auto iter = nodeList.begin(); iter != nodeList.end(); ++iter)
        (*iter)->deleteLater();
    nodeList.clear();

    for (auto iter = deletedNodeList.begin(); iter != deletedNodeList.end(); ++iter)
        (*iter)->deleteLater();
}

NDNodeManager* NDNodeManager::getInstance(void)
{
    static NDNodeManager instance;
    return &instance;
}

// 添加节点（目前不支持节点中添加子节点）
void NDNodeManager::addNode(NDNodeBase* node)
{
    if (node == nullptr)
        return;

    // 设置名字
    if (node->getNodeName().isEmpty())
        node->setNodeName(createNodeName(node->getNodeType()));
    nodeList.append(node);

    emit nodeAdded(node);
}

void NDNodeManager::addNode(NDNodeBase::NodeType type)
{
    NDNodeBase* node = createNode(type);
    if (node == nullptr)
        return;

    nodeList.append(node);

    emit nodeAdded(node);
}

// 删除节点
void NDNodeManager::deleteNode(NDNodeBase* node)
{
    bool finded = false;
    for (auto iter = nodeList.begin(); iter != nodeList.end(); ++iter)
    {
        if (node == *iter)
        {
            deletedNodeList.append(*iter);

            nodeList.erase(iter);
            finded = true;
            break;
        }
    }

    if (finded)
        emit nodeDeleted(node);
}

void NDNodeManager::deleteNode(const QString& nodeName)
{
    NDNodeBase* node = nullptr;
    for (auto iter = nodeList.begin(); iter != nodeList.end(); ++iter)
    {
        if (nodeName == (*iter)->getNodeName())
        {
            node = *iter;
            deletedNodeList.append(*iter);

            nodeList.erase(iter);
            break;
        }
    }

    // 发送删除信号
    if (node)
        emit nodeDeleted(node);
}

void NDNodeManager::setCurrentSelectedNode(NDNodeBase* node)
{
    if (node && m_selectedNode != node)
    {
        m_selectedNode = node;
        emit nodeSelectChanged(m_selectedNode);
    }
}

NDNodeBase* NDNodeManager::createNode(NDNodeBase::NodeType nodeType)
{
    NDNodeBase* node = nullptr;
    if (nodeType == NDNodeBase::t_textNode)
        node = new NDTextNode;

    node->setNodeName(createNodeName(nodeType));
    return node;
}

void NDNodeManager::getNodeList(QList<NDNodeBase*>& nodelist)
{
    nodelist = nodeList;
}

QString NDNodeManager::createNodeName(int type)
{
    QString nodeName = "%1%2";
    if (type == NDNodeBase::t_textNode)
        nodeName = nodeName.arg("textNode").arg(m_pStartCountPtr[(int)NDNodeBase::t_textNode]++);

    return nodeName;
}
