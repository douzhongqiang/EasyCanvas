#ifndef UICANVASITEMMANAGER_H
#define UICANVASITEMMANAGER_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QHash>
#include <QIcon>
#include "easycanvascore_global.h"

class NDNodeBase;
class UICanvasItemBase;
class UICanvasView;

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
        QList<NDNodeBase*> nodes;       // 所有的节点列表
        quint64 count = 0;                  // 计数
        QIcon iconPixmap;
    };

public:
    UICanvasItemManager(QObject* parent = nullptr);
    ~UICanvasItemManager();

    // 创建节点
    UICanvasItemBase* createCanvasItem(CanvasItemType type);
    // 删除节点
    void deleteCanvasItem(NDNodeBase* node);
    void deleteCanvasItem(const QString& nodeName);
    void deleteCanvasItems(const QStringList& nodeNames);
    // 替换节点名称
    bool changedNodeName(const QString& srcName, const QString& destName);
    // 获取所有节点名称
    QStringList getAllNodeNames(void);
    // 查找节点
    NDNodeBase* getNode(const QString& name);
    // 清除所有
    void cleanAll(void);
    // 设置当前的选择列表
    void setSelectedNodes(const QStringList& nodeNames);

    // 设置/获取当前的Canvas
    void setCurrentCanvasView(UICanvasView* canvasView);
    UICanvasView* getCurrentCanvasView(void);

    // 获取当前所有节点类型数目
    int getNodeCounts(void);
    // 获取某种节点类别的数目
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

private:
    QHash<QString, UICanvasItemBase*> m_nameHash;
    QMap<CanvasItemType, TypeNodeInfo> m_countMap;

    // 类型转化为字符串名字
    QString getTypeName(CanvasItemType type);
    // 获取类别的图标
    QIcon getTypeIcon(CanvasItemType type);

    // 多个场景管理器
    static QVector<UICanvasItemManager*> m_canvasList;
    static int m_nCurrentIndex;

    // 当前的CanvasView
    UICanvasView* m_pCanvasView = nullptr;

signals:
    void addedNode(int type, const QString& nodeName);
    void deletedNode(int type, const QString& nodeName);
};

#endif
