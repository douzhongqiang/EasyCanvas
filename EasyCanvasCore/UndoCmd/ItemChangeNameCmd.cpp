#include "ItemChangeNameCmd.h"
#include "UICanvas/UICanvasItemBase.h"
#include "NDNodeBase.h"
#include "UICanvas/UICanvasItemManager.h"

ItemChangeNameCmd::ItemChangeNameCmd(QSharedPointer<UICanvasItemBase> pItem, const QString& destName)
    :m_pCanvasItem(pItem)
    ,m_destName(destName)
{
    m_srcName = m_pCanvasItem->getCurrentNode()->getNodeName();

    QString name = "Changed Node Name %1 To %2";
    this->setText(name.arg(pItem->getCurrentNode()->getNodeName()).arg(destName));
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
