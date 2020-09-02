#ifndef UICANVASITEMMANAGER_H
#define UICANVASITEMMANAGER_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QHash>
#include <QIcon>
#include <QSharedPointer>
#include "easycanvascore_global.h"

class NDNodeBase;
class UICanvasItemBase;
class UICanvasView;
class UndoCmdCore;
class NDAttributeBase;

#define g_currentCanvasManager UICanvasItemManager::getCurrentCanvasManager()

class EASYCANVASCORESHARED_EXPORT UICanvasItemManager : public QObject
{
    Q_OBJECT

public:
    enum CanvasItemType
    {
        t_None,
        t_CanvasItem,
        t_RectItem,
        t_TextItem,
        t_EllipseItem,
        t_ImageItem,
        t_PathItem,
        t_AudioItem,

        t_End
    };

    struct TypeNodeInfo
    {
        quint64 count = 0;                  // 计数
        QIcon iconPixmap;
    };

public:
    UICanvasItemManager(QObject* parent = nullptr);
    ~UICanvasItemManager();

    // 创建节点
    QSharedPointer<UICanvasItemBase> createCanvasItemByCmd(CanvasItemType type);
    QSharedPointer<UICanvasItemBase> createCanvasItem(CanvasItemType type, const QString& nodeName = "", bool isAdded = true);
    // 添加节点
    void addCanvasItem(QSharedPointer<UICanvasItemBase> pCanvasItem);
    // 删除节点
    void deleteCanvasItemByCmd(const QStringList& nodeName);
    void deleteCanvasItem(NDNodeBase* node);
    void deleteCanvasItem(const QString& nodeName);
    void deleteCanvasItems(const QStringList& nodeNames);
    // 替换节点名称
    bool isCanChangedName(const QString& srcName, const QString& destName);
    void changedNodeNameCmd(const QString& srcName, const QString& destName);
    bool changedNodeName(const QString& srcName, const QString& destName);
    // 获取所有节点名称
    QStringList getAllNodeNames(void);
    // 查找节点
    NDNodeBase* getNode(const QString& name);
    QSharedPointer<UICanvasItemBase> getCanvasItem(const QString& name);
    // 清除所有
    void cleanAll(void);
    // 设置当前的选择列表
    void setSelectedNodes(const QStringList& nodeNames);
    QStringList getSelectedNodes(void);

    // 复制选中节点
    void copySelectNodes(void);
    // 粘贴
    void pasteCmd(void);
    QList<QSharedPointer<UICanvasItemBase>> paste(void);

    // 批量修该节点属性
    void changedAttributeValues(const QList<NDAttributeBase*>& attributes, \
                                const QVector<QVariant>& values, bool isCmd = false);

    // 设置/获取当前的Canvas
    void setCurrentCanvasView(UICanvasView* canvasView);
    UICanvasView* getCurrentCanvasView(void);

    // 获取当前所有节点类型数目
    int getNodeCounts(void);
    // 获取某种节点类别的数目(当前索引的计数)
    int getNodeCounts(CanvasItemType type);

    // 获取节点类别的名字
    QString getNodeTypeDisplayName(CanvasItemType type);
    // 获取节点类别的图标
    QIcon getNodeTypeDisplayIcon(CanvasItemType type);

    static UICanvasItemManager* getCurrentCanvasManager(void);
    // 创建CanvasManager
    static void createCanvasManager(void);
    // 设置/获取当前选择
    static void setCurrentIndex(int index);
    static int getCurrentIndex(void);

public:
    // 撤銷和重做相关
    void redo(void);
    void undo(void);

    // 获取Redo/Undo核心类
    UndoCmdCore* getCurrentUndoCmdCore(void);

public:
    // 类型转化为字符串名字
    QString getTypeName(CanvasItemType type);
    CanvasItemType getTypeByName(const QString& name);

    // 设置当前最大ID
    void setCurrentMaxId(int maxId);

    // 根节点（画布节点）
    QJsonObject getNodeStoreJson(NDNodeBase* pNode);
    void fillNodeJsonInfo(NDNodeBase* pNode, const QJsonObject& jsonObject);

private:
    int m_nMaxIndex = 1;    // 最大索引

    QHash<QString, QSharedPointer<UICanvasItemBase>> m_nameHash;
    QMap<CanvasItemType, TypeNodeInfo> m_countMap;

    // 获取类别的图标
    QIcon getTypeIcon(CanvasItemType type);

    // 多个场景管理器
    static QVector<UICanvasItemManager*> m_canvasList;
    static int m_nCurrentIndex;

    // 当前的CanvasView
    UICanvasView* m_pCanvasView = nullptr;
    // UodoCmdCore
    UndoCmdCore* m_pUndoCmdCore = nullptr;
    QSharedPointer<UICanvasItemBase> m_pCanvasItemBase;

    // 节点复制相关
    QString m_cCopyString;

signals:
    void addedNode(int type, const QString& nodeName);
    void deletedNode(int type, const QString& nodeName);
    void changeNodeName(int type, const QString& srcName, const QString& destName);

private slots:
    void onAttributeValueChanged(NDAttributeBase* pAttribute, const QVariant& value, bool cmd);
};

#endif
