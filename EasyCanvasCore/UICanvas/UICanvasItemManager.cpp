#include "UICanvasItemManager.h"
#include "UICanvasItemBase.h"
#include "NDNodeBase.h"
#include "UICanvasRectItem.h"
#include "UICanvasEllipseItem.h"
#include "UICanvasTextItem.h"
#include "UICanvasAudioItem.h"
#include "UICanvasPathItem.h"
#include "UICanvasImageItem.h"

QVector<UICanvasItemManager*> UICanvasItemManager::m_canvasList;
int UICanvasItemManager::m_nCurrentIndex = -1;

UICanvasItemManager::UICanvasItemManager(QObject* parent)
    :QObject(parent)
{
    for (int var = (int)t_RectItem; var < (int)t_End; ++var)
    {
        m_countMap[(CanvasItemType)var].count = 0;
        m_countMap[(CanvasItemType)var].iconPixmap = getTypeIcon((CanvasItemType)var);
    }
}

UICanvasItemManager::~UICanvasItemManager()
{

}

QString UICanvasItemManager::getTypeName(CanvasItemType type)
{
    switch (type)
    {
    case t_RectItem:
        return "RectNode";
    case t_TextItem:
        return "TextNode";
    case t_EllipseItem:
        return "EllipseNode";
    case t_ImageItem:
        return "ImageNode";
    case t_PathItem:
        return "PathNode";
    case t_AudioItem:
        return "AudioNode";
    default:
        return "";
    }

    return "";
}

// 获取类别的图标
QIcon UICanvasItemManager::getTypeIcon(CanvasItemType type)
{
    switch (type)
    {
    case t_RectItem:
        return QIcon("./images/rectItem.png");
    case t_TextItem:
        return QIcon("./images/textItem.png");
    case t_EllipseItem:
        return QIcon("./images/ellipseItem.png");
    case t_ImageItem:
        return QIcon("./images/imageItem.png");
    case t_PathItem:
        return QIcon("./images/freeDrawItem.png");
    case t_AudioItem:
        return QIcon("./images/audioItem.png");
    default:
        return QIcon();
    }

    return QIcon();
}

UICanvasItemBase* UICanvasItemManager::createCanvasItem(CanvasItemType type)
{
    UICanvasItemBase* pItem = nullptr;
    switch (type)
    {
    case t_RectItem:
    {
        pItem = new UICanvasRectItem;
        break;
    }
    case t_TextItem:
    {
        pItem = new UICanvasTextItem;
        break;
    }
    case t_EllipseItem:
    {
        pItem = new UICanvasEllipseItem;
        break;
    }
    case t_ImageItem:
    {
        pItem = new UICanvasImageItem;
        break;
    }
    case t_PathItem:
    {
        pItem = new UICanvasPathItem;
        break;
    }
    case t_AudioItem:
    {
        pItem = new UICanvasAudioItem;
        break;
    }
    default:
        break;
    }

    if (pItem == nullptr)
        return pItem;

    // 设置节点名称
    NDNodeBase* pNode = pItem->getCurrentNode();
    QString nodeName = QString("%1_%2").arg(getTypeName(type)).arg(m_countMap[type].count++);
    pNode->setNodeName(nodeName);
    // 添加到哈希表中
    m_nameHash.insert(nodeName, pItem);

    // 发送添加信号
    emit addedNode(type, nodeName);

    return pItem;
}

// 删除节点
void UICanvasItemManager::deleteCanvasItem(NDNodeBase* node)
{
    if (node == nullptr)
        return;

    QString name = node->getNodeName();
    m_nameHash.remove(name);
}

void UICanvasItemManager::deleteCanvasItem(const QString& nodeName)
{
    m_nameHash.remove(nodeName);
}

QStringList UICanvasItemManager::getAllNodeNames(void)
{
    return m_nameHash.keys();
}

NDNodeBase* UICanvasItemManager::getNode(const QString& name)
{
    const auto iter = m_nameHash.find(name);
    if (iter == m_nameHash.end())
        return nullptr;

    return (*iter)->getCurrentNode();
}

int UICanvasItemManager::getNodeCounts(void)
{
    return (int)t_End - (int)t_None - 2;
}

int UICanvasItemManager::getNodeCounts(CanvasItemType type)
{
    if (type == t_CanvasItem)
        return getNodeCounts();

    return m_countMap[type].count;
}

QString UICanvasItemManager::getNodeTypeDisplayName(CanvasItemType type)
{
    return getTypeName(type);
}

QIcon UICanvasItemManager::getNodeTypeDisplayIcon(CanvasItemType type)
{
    auto iter = m_countMap.find(type);
    if (iter == m_countMap.end())
        return QIcon();

    return iter.value().iconPixmap;
}

UICanvasItemManager* UICanvasItemManager::getCurrentCanvasManager(void)
{
    return m_canvasList[m_nCurrentIndex];
}

void UICanvasItemManager::createCanvasManager(void)
{
    UICanvasItemManager* manager = new UICanvasItemManager;
    m_canvasList.append(manager);
}

// 设置/获取当前选择
void UICanvasItemManager::setCurrentIndex(int index)
{
    m_nCurrentIndex = index;
}

int UICanvasItemManager::getCurrentIndex(void)
{
    return m_nCurrentIndex;
}
