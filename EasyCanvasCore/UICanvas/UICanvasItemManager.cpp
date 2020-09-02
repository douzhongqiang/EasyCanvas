#include "UICanvasItemManager.h"
#include "UICanvasItemBase.h"
#include "NDNodeBase.h"
#include "UICanvasRectItem.h"
#include "UICanvasEllipseItem.h"
#include "UICanvasTextItem.h"
#include "UICanvasAudioItem.h"
#include "UICanvasPathItem.h"
#include "UICanvasImageItem.h"
#include "UICanvasView.h"
#include "UndoCmd/UndoCmdCore.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <QJsonParseError>
#include "NDNodeManager.h"

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

    m_pUndoCmdCore = new UndoCmdCore(this);

    QObject::connect(g_nodeManager, &NDNodeManager::signalAttrValueChanged, \
                     this, &UICanvasItemManager::onAttributeValueChanged);
}

UICanvasItemManager::~UICanvasItemManager()
{

}

QString UICanvasItemManager::getTypeName(CanvasItemType type)
{
    switch (type)
    {
    case t_CanvasItem:
        return "CanvasNode";
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

UICanvasItemManager::CanvasItemType UICanvasItemManager::getTypeByName(const QString& name)
{
    if (name == "CanvasNode")
        return t_CanvasItem;
    else if (name == "RectNode")
        return t_RectItem;
    else if (name == "TextNode")
        return t_TextItem;
    else if (name == "EllipseNode")
        return t_EllipseItem;
    else if (name == "ImageNode")
        return t_ImageItem;
    else if (name == "PathNode")
        return t_PathItem;
    else if (name == "AudioNode")
        return t_AudioItem;

    return t_None;
}

void UICanvasItemManager::setCurrentMaxId(int maxId)
{
    m_nMaxIndex = maxId;
}

// 根节点（画布节点）
QJsonObject UICanvasItemManager::getNodeStoreJson(NDNodeBase* pNode)
{
    if (pNode == nullptr)
        return QJsonObject();

    if (pNode == m_pCanvasView->getCurrentSceneNode())
    {
        QJsonObject object;
        object.insert("name", pNode->getNodeName());
        object.insert("type", pNode->getNodeType());

        // 获取属性组
        QList<NDAttributeGroup*> attrGroups;
        pNode->getAllAttributeGroups(attrGroups);

        QJsonObject attrObject;
        for (auto iter = attrGroups.begin(); iter != attrGroups.end(); ++iter)
        {
            // 获取属性列表
            QList<NDAttributeBase*> attrs;
            (*iter)->getAttributes(attrs);

            for (auto attrIter = attrs.begin(); attrIter != attrs.end(); ++attrIter)
            {
                QString attrName = (*attrIter)->getName();
                QVariant value = (*attrIter)->getValue();

                attrObject.insert(attrName, QJsonValue::fromVariant(value));
            }
        }

        object.insert("attribute", attrObject);
        return object;
    }

    auto item = getCanvasItem(pNode->getNodeName());
    return item->getStoreJson();
}

void UICanvasItemManager::fillNodeJsonInfo(NDNodeBase* pNode, const QJsonObject& jsonObject)
{
    if (pNode == nullptr)
        return;

    if (pNode == m_pCanvasView->getCurrentSceneNode())
    {
        QJsonObject object = jsonObject.value("attribute").toObject();

        for (auto iter = object.begin(); iter != object.end(); ++iter)
        {
            QString attrName = iter.key();
            QVariant value = iter.value().toVariant();

            NDAttributeBase* pAttr = pNode->getAttribute(attrName);
            if (pAttr == nullptr)
                continue;

            pAttr->setValue(value);
        }
        return;
    }

    auto item = getCanvasItem(pNode->getNodeName());
    return item->fillJsonInfo(jsonObject);
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

QSharedPointer<UICanvasItemBase> UICanvasItemManager::createCanvasItem(CanvasItemType type, const QString& nodeName, bool isAdded)
{
    m_pCanvasItemBase.reset();

    switch (type)
    {
    case t_RectItem:
    {
        m_pCanvasItemBase = QSharedPointer<UICanvasItemBase>(new UICanvasRectItem);
        break;
    }
    case t_TextItem:
    {
        m_pCanvasItemBase = QSharedPointer<UICanvasItemBase>(new UICanvasTextItem);
        break;
    }
    case t_EllipseItem:
    {
        m_pCanvasItemBase = QSharedPointer<UICanvasItemBase>(new UICanvasEllipseItem);
        break;
    }
    case t_ImageItem:
    {
        m_pCanvasItemBase = QSharedPointer<UICanvasItemBase>(new UICanvasImageItem);
        break;
    }
    case t_PathItem:
    {
        m_pCanvasItemBase = QSharedPointer<UICanvasItemBase>(new UICanvasPathItem);
        break;
    }
    case t_AudioItem:
    {
        m_pCanvasItemBase = QSharedPointer<UICanvasItemBase>(new UICanvasAudioItem);
        break;
    }
    default:
        break;
    }

    if (m_pCanvasItemBase == nullptr || !isAdded)
        return m_pCanvasItemBase;

    // 设置位置和选中
    m_pCanvasView->cleanAllSelected();
    m_pCanvasItemBase->setSelected(true);
    m_pCanvasItemBase->setCurrentIndex(m_nMaxIndex++);

    // 获取属性名
    QString tempNodeName = nodeName;
    if (tempNodeName.isEmpty() || m_nameHash.find(tempNodeName) != m_nameHash.end())
    {
        // 自动生成节点名
        do {
            tempNodeName = QString("%1_%2").arg(getTypeName(type)).arg(m_countMap[type].count++);
        }while(m_nameHash.find(tempNodeName) != m_nameHash.end());
    }

    // 设置属性名和类型
    NDNodeBase* pNode = m_pCanvasItemBase->getCurrentNode();
    pNode->setNodeName(tempNodeName);
    pNode->setNodeType((int)type);

    m_pCanvasView->addToScene(m_pCanvasItemBase.data());

    // 添加到哈希表中
    m_nameHash.insert(tempNodeName, m_pCanvasItemBase);

    // 发送添加信号
    emit addedNode(type, tempNodeName);

    return m_pCanvasItemBase;
}

void UICanvasItemManager::addCanvasItem(QSharedPointer<UICanvasItemBase> pCanvasItem)
{
    if (pCanvasItem == nullptr)
        return;

    CanvasItemType type = (CanvasItemType)pCanvasItem->getCurrentNode()->getNodeType();

    // 获取属性名
    QString tempNodeName = pCanvasItem->getCurrentNode()->getNodeName();
    if (tempNodeName.isEmpty() || m_nameHash.find(tempNodeName) != m_nameHash.end())
    {
        // 自动生成节点名
        do {
            tempNodeName = QString("%1_%2").arg(getTypeName(type)).arg(m_countMap[type].count++);
        }while(m_nameHash.find(tempNodeName) != m_nameHash.end());
    }

    // 设置属性名和类型
    NDNodeBase* pNode = pCanvasItem->getCurrentNode();
    pNode->setNodeName(tempNodeName);

    m_pCanvasView->addToScene(pCanvasItem.data());

    // 添加到哈希表中
    m_nameHash.insert(tempNodeName, pCanvasItem);

    // 发送添加信号
    emit addedNode(pCanvasItem->getCurrentNode()->getNodeType(), tempNodeName);
}

QSharedPointer<UICanvasItemBase> UICanvasItemManager::createCanvasItemByCmd(CanvasItemType type)
{
    m_pUndoCmdCore->runCreateCmd((int)type);
    return m_pCanvasItemBase;
}


void UICanvasItemManager::deleteCanvasItemByCmd(const QStringList& nodeName)
{
    m_pUndoCmdCore->runDeleteCmd(nodeName);
}

// 删除节点
void UICanvasItemManager::deleteCanvasItem(NDNodeBase* node)
{
    if (node == nullptr)
        return;

    QString name = node->getNodeName();
    deleteCanvasItem(name);
}

void UICanvasItemManager::deleteCanvasItem(const QString& nodeName)
{
    auto iter = m_nameHash.find(nodeName);
    if (iter == m_nameHash.end())
        return;

    // 刪除
    auto canvasItem = iter.value();
    int type = canvasItem->getCurrentNode()->getNodeType();

    // 从场景中移除元素
    m_pCanvasView->removeFromScene(canvasItem.data());

    // 移除
    m_nameHash.remove(nodeName);

    emit deletedNode(type, nodeName);
}

void UICanvasItemManager::deleteCanvasItems(const QStringList& nodeNames)
{
    foreach (const QString& nodeName, nodeNames)
    {
        deleteCanvasItem(nodeName);
    }
}

void UICanvasItemManager::changedNodeNameCmd(const QString& srcName, const QString& destName)
{
    m_pUndoCmdCore->runChangeNameCmd(srcName, destName);
}

bool UICanvasItemManager::isCanChangedName(const QString& srcName, const QString& destName)
{
    // 已存在该节点
    if (m_nameHash.find(destName) != m_nameHash.end())
    {
        return false;
    }

    // 查找节点
    auto iter = m_nameHash.find(srcName);
    if (iter == m_nameHash.end())
        return false;

    return true;
}

bool UICanvasItemManager::changedNodeName(const QString& srcName, const QString& destName)
{
    // 已存在该节点
    if (m_nameHash.find(destName) != m_nameHash.end())
    {
        qDebug() << "Node Name is Existed";
        return false;
    }

    // 查找节点
    auto iter = m_nameHash.find(srcName);
    if (iter == m_nameHash.end())
        return false;

    // 设置节点名字
    auto canvasItem = iter.value();
    NDNodeBase* pNode = canvasItem->getCurrentNode();
    pNode->setNodeName(destName);

    m_nameHash.remove(srcName);
    m_nameHash.insert(destName, canvasItem);

    // 发送名字更改信号
    emit changeNodeName(pNode->getNodeType(), srcName, destName);

    return true;
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

QSharedPointer<UICanvasItemBase> UICanvasItemManager::getCanvasItem(const QString& name)
{
    const auto iter = m_nameHash.find(name);
    if (iter == m_nameHash.end())
        return nullptr;

    return *iter;
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

// 设置当前的Canvas
void UICanvasItemManager::setCurrentCanvasView(UICanvasView* canvasView)
{
    m_pCanvasView = canvasView;
}

UICanvasView* UICanvasItemManager::getCurrentCanvasView(void)
{
    return m_pCanvasView;
}

void UICanvasItemManager::cleanAll(void)
{
    // 重置根节点
    m_pCanvasView->resetSceneNode();

    m_cCopyString = "";
    m_nMaxIndex = 1;

    // 删除所有节点
    for (auto iter = m_nameHash.begin(); iter != m_nameHash.end(); ++iter)
    {
        UICanvasItemBase* pItem = iter.value().data();
        m_pCanvasView->removeFromScene(pItem);

        NDNodeBase* pNode = pItem->getCurrentNode();
        emit deletedNode(pNode->getNodeType(), pNode->getNodeName());
    }

    //清空 Redo/Undo栈
    m_pUndoCmdCore->cleanUndoStack();
    m_nameHash.clear();

    // 重新计数
    for (auto iter = m_countMap.begin(); iter != m_countMap.end(); ++iter)
        iter->count = 0;
    m_pCanvasItemBase.reset();
}

void UICanvasItemManager::changedAttributeValues(const QList<NDAttributeBase*>& attributes, \
                            const QVector<QVariant>& values, bool isCmd)
{
    if (isCmd)
    {
        m_pUndoCmdCore->runChangedAttrCmd(attributes, values, true);
        return;
    }

    int count = 0;
    for (auto iter = attributes.begin(); iter != attributes.end(); ++iter)
    {
        (*iter)->setValue(values[count++]);
    }
}

void UICanvasItemManager::setSelectedNodes(const QStringList& nodeNames)
{
    m_pCanvasView->cleanAllSelected();

    foreach(const QString& nodeName, nodeNames)
    {
        auto iter = m_nameHash.find(nodeName);
        if (iter == m_nameHash.end())
            continue;

        (*iter)->setSelected(true);
    }
}

QStringList UICanvasItemManager::getSelectedNodes(void)
{
    QStringList nodeNames;
    QList<NDNodeBase*> nodes = m_pCanvasView->getCurrentSelectedNodes();
    for (auto iter = nodes.begin(); iter != nodes.end(); ++iter)
        nodeNames << (*iter)->getNodeName();

    return nodeNames;
}

// 复制选中节点
void UICanvasItemManager::copySelectNodes(void)
{
    QJsonArray jsonArray;
    QList<UICanvasItemBase*> selectItems = m_pCanvasView->getCurrentSelectedItems();

    int count = 0;
    for (auto iter = selectItems.begin(); iter != selectItems.end(); ++iter)
    {
        jsonArray.insert(count++, (*iter)->getStoreJson(30));
    }

    // Json转为字符串
    QJsonDocument doc(jsonArray);
    QByteArray byteArray = doc.toJson(QJsonDocument::Compact);
    QString jsonString(byteArray);

    m_cCopyString = jsonString;
}

void UICanvasItemManager::pasteCmd(void)
{
    m_pUndoCmdCore->runPasteCmd();
}

QList<QSharedPointer<UICanvasItemBase>> UICanvasItemManager::paste(void)
{
    QList<QSharedPointer<UICanvasItemBase>> itemList;
    QJsonParseError jsonParseError;
    QJsonDocument doc = QJsonDocument::fromJson(m_cCopyString.toUtf8(), &jsonParseError);
    if (jsonParseError.error != QJsonParseError::NoError)
        return itemList;

    QJsonArray array = doc.array();
    for (auto iter = array.begin(); iter != array.end(); ++iter)
    {
        QJsonObject object = iter->toObject();

        // 创建元素
        CanvasItemType canvasType = (CanvasItemType)object.value("type").toInt();
        QSharedPointer<UICanvasItemBase> canvasItem = this->createCanvasItem(canvasType);

        // 设置元素属性
        canvasItem->fillJsonInfo(object);

        itemList << canvasItem;
    }

    return itemList;
}

// 撤銷和重做相关
void UICanvasItemManager::redo(void)
{
    m_pUndoCmdCore->redo();
}

void UICanvasItemManager::undo(void)
{
    m_pUndoCmdCore->undo();
}

// 获取Redo/Undo核心类
UndoCmdCore* UICanvasItemManager::getCurrentUndoCmdCore(void)
{
    return m_pUndoCmdCore;
}

void UICanvasItemManager::onAttributeValueChanged(NDAttributeBase* pAttribute, const QVariant& value, bool cmd)
{
    if (!cmd)
        return;

    QList<NDAttributeBase*> attributeList;
    attributeList << pAttribute;
    m_pUndoCmdCore->runChangedAttrCmd(attributeList, value);
}
