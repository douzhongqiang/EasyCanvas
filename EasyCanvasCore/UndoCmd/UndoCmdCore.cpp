#include "UndoCmdCore.h"
#include "ItemCreateCmd.h"
#include "ItemDeleteCmd.h"
#include "ItemChangeNameCmd.h"
#include "ItemPasteCmd.h"
#include "ItemAttributeChangedCmd.h"
#include "NDNodeBase.h"
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
void UndoCmdCore::runChangedAttrCmd(const QString& attrFullName, const QVariant& value, bool isCanrun)
{
    int index = attrFullName.indexOf(".");
    if (index < 0)
        return;

    QString nodeName = attrFullName.left(index);
    QString attrName = attrFullName.right(attrFullName.length() - index - 1);

    NDNodeBase* node = g_currentCanvasManager->getNode(nodeName);
    if (node == nullptr)
        return;

    NDAttributeBase* pAttribute = node->getAttribute(attrName);
    if (pAttribute == nullptr)
        return;

    QList<NDAttributeBase*> attributeList;
    attributeList << pAttribute;
    ItemAttributeChangedCmd* changedAttr = new ItemAttributeChangedCmd(attributeList, value, isCanrun);
    m_pUndoStack->push(changedAttr);
}

void UndoCmdCore::runChangedAttrCmd(const QList<NDAttributeBase*>& attrList, const QVariant& value, bool isCanrun)
{
    ItemAttributeChangedCmd* changedAttr = new ItemAttributeChangedCmd(attrList, value, isCanrun);
    m_pUndoStack->push(changedAttr);
}

void UndoCmdCore::runChangedAttrCmd(const QList<NDAttributeBase*>& attrList, \
                                    const QVector<QVariant>& values, bool isCanrun)
{
    ItemAttributeChangedCmd* changedAttr = new ItemAttributeChangedCmd(attrList, values, isCanrun);
    m_pUndoStack->push(changedAttr);
}

void UndoCmdCore::runPasteCmd(void)
{
    ItemPasteCmd* pasteCmd = new ItemPasteCmd;
    m_pUndoStack->push(pasteCmd);
}

void UndoCmdCore::redo(void)
{
    m_pUndoStack->redo();
}

void UndoCmdCore::undo(void)
{
    m_pUndoStack->undo();
}

// 创建Redo/Undo Action
QAction* UndoCmdCore::createRedoAction(void)
{
    return m_pUndoStack->createRedoAction(this, tr("Redo"));
}

QAction* UndoCmdCore::createUndoAction(void)
{
    return m_pUndoStack->createUndoAction(this, tr("Undo"));
}

QUndoView* UndoCmdCore::createUndoView(void)
{
    QUndoView* pView = new QUndoView(m_pUndoStack);
    return pView;
}

bool UndoCmdCore::isUndoStackEmpty(void)
{
    return m_pUndoStack->index() <= 0;
}

void UndoCmdCore::cleanUndoStack(void)
{
    m_pUndoStack->clear();
}
