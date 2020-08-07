#include "UINodeItem.h"

UINodeItem::UINodeItem(UINodeItem* parentItem)
    :m_pParentItem(parentItem)
{

}

UINodeItem::~UINodeItem()
{

}

// 添加子节点
void UINodeItem::appendChildNode(UINodeItem* nodeItem)
{
    nodeItem->m_pParentItem = this;
    m_childItems.append(nodeItem);
}

UINodeItem* UINodeItem::getChildNode(int index)
{
    if (m_childItems.count() <= index)
        return nullptr;

    return m_childItems[index];
}

// 设置/获取父节点
void UINodeItem::setParentNode(UINodeItem* node)
{
    m_pParentItem = node;
}

UINodeItem* UINodeItem::getParentNode(void)
{
    return m_pParentItem;
}

// 设置/获取节点名字
void UINodeItem::setName(const QString& name)
{
    m_cDisplayName = name;
}

QString UINodeItem::getName(void)
{
    return m_cDisplayName;
}

void UINodeItem::setCanvasNodeVisible(bool isNode)
{
    m_isCanvasNode = isNode;
}

bool UINodeItem::isCanvasNode(void)
{
    return m_isCanvasNode;
}

// 设置/获取节点类型
void UINodeItem::setNodeType(int type)
{
    m_nNodeType = type;
}

int UINodeItem::getNodeType(void)
{
    return m_nNodeType;
}

// 获取当前的索引
int UINodeItem::getCurrentIndex(void)
{
    if (m_pParentItem == nullptr)
        return 0;

    int count = 0;
    for (auto iter = m_pParentItem->m_childItems.begin(); iter != m_pParentItem->m_childItems.end(); ++iter)
    {
        if (*iter == this)
            return count;

        count++;
    }

    return -1;
}

int UINodeItem::getCurrentIndex(UINodeItem* node)
{
    int count = 0;
    for (auto iter = m_childItems.begin(); iter != m_childItems.end(); ++iter)
    {
        if (*iter == node)
            return count;

        count++;
    }

    return -1;
}

// 获取子元素的数量
int UINodeItem::getSubNodeCount(void)
{
    return m_childItems.count();
}
