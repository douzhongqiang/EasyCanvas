#include "UINodeTreeViewModel.h"
#include "UINodeItem.h"
#include "UICanvas/UICanvasItemManager.h"

UINodeTreeViewModel::UINodeTreeViewModel(QObject* parent)
    :QAbstractItemModel(parent)
{
    m_pCanvasItemData = g_currentCanvasManager;
    updateAllNodes();

    QObject::connect(m_pCanvasItemData, &UICanvasItemManager::addedNode, this, &UINodeTreeViewModel::onAddedNodeItem);
}

UINodeTreeViewModel::~UINodeTreeViewModel()
{

}

QModelIndex UINodeTreeViewModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!this->hasIndex(row, column, parent))
        return QModelIndex();

    if (!parent.isValid())
    {
        // 根节点
        return createIndex(row, column, m_pRootNodeItem);
    }

    // 创建普通节点
    UINodeItem* nodeItem = static_cast<UINodeItem*>(parent.internalPointer());
    if (nodeItem == nullptr)
        return QModelIndex();

    // 获取当前的子节点
    UINodeItem* childNode = nodeItem->getChildNode(row);
    if (childNode == nullptr)
        return QModelIndex();

    // 创建节点
    return createIndex(row, column, childNode);
}

QModelIndex UINodeTreeViewModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    // 获取父节点
    UINodeItem* nodeItem = static_cast<UINodeItem*>(child.internalPointer());
    nodeItem = nodeItem->getParentNode();
    if (nodeItem == nullptr)
        return QModelIndex();

    // 获取父节点的索引
    int row = nodeItem->getCurrentIndex();
    return createIndex(row, 0, nodeItem);
}

int UINodeTreeViewModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return 1;
    }

    UINodeItem* pNode = static_cast<UINodeItem*>(parent.internalPointer());
    return pNode->getSubNodeCount();
}

int UINodeTreeViewModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant UINodeTreeViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    UINodeItem* pNode = static_cast<UINodeItem*>(index.internalPointer());
    if (role == Qt::DisplayRole)
    {
        return pNode->getName();
    }
    else if (role == Qt::DecorationRole)
    {
        if (pNode->isCanvasNode())
        {
            return m_pCanvasItemData->getNodeTypeDisplayIcon( \
                        (UICanvasItemManager::CanvasItemType)pNode->getNodeType());
        }
    }

    return QVariant();
}

Qt::ItemFlags UINodeTreeViewModel::flags(const QModelIndex &index) const
{
    UINodeItem* pNode = static_cast<UINodeItem*>(index.internalPointer());
    if (!pNode->isCanvasNode())
        return QAbstractItemModel::flags(index);

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

void UINodeTreeViewModel::updateAllNodes(void)
{
    // 创建根节点
    m_pRootNodeItem = new UINodeItem;
    m_pRootNodeItem->setName(tr("CanvasNode"));

    int nodeTypeCount = m_pCanvasItemData->getNodeCounts();
    for (int i=0; i<nodeTypeCount; i++)
    {
        // 创建节点类型节点
        UINodeItem* typeNode = new UINodeItem(m_pRootNodeItem);
        m_pRootNodeItem->appendChildNode(typeNode);

        // 获取类别的显示名字
        QString typeName = m_pCanvasItemData->getNodeTypeDisplayName(\
                    (UICanvasItemManager::CanvasItemType)(i + (int)UICanvasItemManager::t_RectItem));
        typeNode->setName(typeName);
    }
}

// 设置当前的TreeView
void UINodeTreeViewModel::setCurrentTreeView(QTreeView* pTreeView)
{
    m_pTreeView = pTreeView;
}

bool UINodeTreeViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    UINodeItem* pNode = static_cast<UINodeItem*>(index.internalPointer());
    pNode->setName(value.toString());

    return true;
}

void UINodeTreeViewModel::onAddedNodeItem(int nodeType, const QString& name)
{
    UINodeItem* node = m_pRootNodeItem->getChildNode(nodeType - 2);
    if (node == nullptr)
        return;

    UINodeItem* childNode = new UINodeItem;
    childNode->setName(name);
    childNode->setCanvasNodeVisible(true);
    childNode->setNodeType(nodeType);

    node->appendChildNode(childNode);

    // 更新显示
    m_pTreeView->doItemsLayout();
}

void UINodeTreeViewModel::onDeleteNodeItem(int nodeType, const QString& name)
{

}
