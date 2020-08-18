#include "ItemChangeNameCmd.h"
#include "UICanvas/UICanvasItemBase.h"
#include "NDNodeBase.h"
#include "UICanvas/UICanvasItemManager.h"

ItemChangeNameCmd::ItemChangeNameCmd(QSharedPointer<UICanvasItemBase> pItem, const QString& destName)
    :m_pCanvasItem(pItem)
    ,m_destName(destName)
{
    m_srcName = m_pCanvasItem->getCurrentNode()->getNodeName();
}

ItemChangeNameCmd::~ItemChangeNameCmd()
{

}

void ItemChangeNameCmd::undo()
{
    g_currentCanvasManager->changedNodeName(m_destName, m_srcName);
}

void ItemChangeNameCmd::redo()
{
    g_currentCanvasManager->changedNodeName(m_srcName, m_destName);
}
