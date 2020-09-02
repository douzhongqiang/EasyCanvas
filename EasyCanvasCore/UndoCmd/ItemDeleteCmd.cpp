#include "ItemDeleteCmd.h"
#include "UICanvas/UICanvasItemManager.h"
#include "UICanvas/UICanvasItemBase.h"
#include "NDNodeBase.h"

ItemDeleteCmd::ItemDeleteCmd(QList<QSharedPointer<UICanvasItemBase>> pItems)
    :m_pCanvasItems(pItems)
{
    // 设置名字
    QString deleteNodeString = "Delete Node [%1]";
    QStringList strs;
    for (auto iter = pItems.begin(); iter != pItems.end(); ++iter)
    {
        strs << (*iter)->getCurrentNode()->getNodeName();
    }
    this->setText(deleteNodeString.arg(strs.join(",")));
}

ItemDeleteCmd::~ItemDeleteCmd()
{

}

void ItemDeleteCmd::undo(void)
{
    foreach (auto canvasItem, m_pCanvasItems)
    {
        if (canvasItem.isNull())
            continue;

        g_currentCanvasManager->addCanvasItem(canvasItem);
    }
}

void ItemDeleteCmd::redo(void)
{
    QStringList nodeNames;
    foreach (auto canvasItem, m_pCanvasItems)
    {
        if (canvasItem.isNull())
            continue;

        nodeNames << canvasItem->getCurrentNode()->getNodeName();
    }

    g_currentCanvasManager->deleteCanvasItems(nodeNames);

}
