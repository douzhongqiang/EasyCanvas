#include "ItemPasteCmd.h"
#include "UICanvas/UICanvasItemManager.h"
#include "UICanvas/UICanvasItemBase.h"

ItemPasteCmd::ItemPasteCmd()
{
    this->setText("Paste Cmd");
}

ItemPasteCmd::~ItemPasteCmd()
{

}

void ItemPasteCmd::undo(void)
{
    for (auto iter = m_createdItems.begin(); iter != m_createdItems.end(); ++iter)
    {
        NDNodeBase* pNode = (*iter)->getCurrentNode();
        g_currentCanvasManager->deleteCanvasItem(pNode);
    }
}

void ItemPasteCmd::redo(void)
{
    if (m_createdItems.size() <= 0)
        m_createdItems = g_currentCanvasManager->paste();
    else {
        for (auto iter = m_createdItems.begin(); iter != m_createdItems.end(); ++iter)
        {
            g_currentCanvasManager->addCanvasItem(*iter);
        }
    }
}
