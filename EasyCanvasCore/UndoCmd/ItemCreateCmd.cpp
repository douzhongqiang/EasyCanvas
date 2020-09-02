#include "ItemCreateCmd.h"
#include "UICanvas/UICanvasItemManager.h"
#include "UICanvas/UICanvasItemBase.h"

ItemCreateCmd::ItemCreateCmd(int type)
    :m_nType(type)
{
    m_itemCanvasItem.reset();

    QString str("Create Node %1");
    this->setText(str.arg(g_currentCanvasManager->getNodeTypeDisplayName((UICanvasItemManager::CanvasItemType)type)));
}

ItemCreateCmd::~ItemCreateCmd()
{

}

void ItemCreateCmd::undo()
{
    if (m_itemCanvasItem)
    {
        NDNodeBase* pNode = m_itemCanvasItem->getCurrentNode();
        g_currentCanvasManager->deleteCanvasItem(pNode);
    }
}

void ItemCreateCmd::redo()
{
    if (m_itemCanvasItem)
    {
        g_currentCanvasManager->addCanvasItem(m_itemCanvasItem);
    }
    else
    {
        UICanvasItemManager::CanvasItemType itemType = (UICanvasItemManager::CanvasItemType)m_nType;
        m_itemCanvasItem = g_currentCanvasManager->createCanvasItem(itemType);
    }
}
