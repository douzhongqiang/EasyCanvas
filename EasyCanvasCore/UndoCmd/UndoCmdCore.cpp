#include "UndoCmdCore.h"
#include "ItemCreateCmd.h"
#include "ItemDeleteCmd.h"
#include "ItemChangeNameCmd.h"
#include "UICanvas/UICanvasItemManager.h"

UndoCmdCore::UndoCmdCore(QObject* parent)
    :QObject(parent)
{
    m_pUndoStack = new QUndoStack(this);
}

UndoCmdCore::~UndoCmdCore()
{

}

// 添加 创建节点命令
void UndoCmdCore::runCreateCmd(int type)
{
    ItemCreateCmd* itemCreateCmd = new ItemCreateCmd(type);
    m_pUndoStack->push(itemCreateCmd);
}

// 添加 删除节点命令
void UndoCmdCore::runDeleteCmd(const QStringList& nodeNames)
{
    // 获取节点元素列表
    QList<QSharedPointer<UICanvasItemBase>> canvasItems;
    foreach (const QString& nodeName, nodeNames)
    {
        auto canvasItem = g_currentCanvasManager->getCanvasItem(nodeName);
        canvasItems << canvasItem;
    }

    ItemDeleteCmd* itemDeleteCmd = new ItemDeleteCmd(canvasItems);
    m_pUndoStack->push(itemDeleteCmd);
}

// 添加 改变名字命令
void UndoCmdCore::runChangeNameCmd(const QString& nodeName, const QString& destNodeName)
{
    auto canvasItem = g_currentCanvasManager->getCanvasItem(nodeName);
    ItemChangeNameCmd* changeNameCmd = new ItemChangeNameCmd(canvasItem, destNodeName);
    m_pUndoStack->push(changeNameCmd);
}

// 添加 改变属性命令
void UndoCmdCore::runChangedAttrCmd(const QString& attrFullName, const QVariant& value)
{

}

void UndoCmdCore::redo(void)
{
    m_pUndoStack->redo();
}

void UndoCmdCore::undo(void)
{
    m_pUndoStack->undo();
}
