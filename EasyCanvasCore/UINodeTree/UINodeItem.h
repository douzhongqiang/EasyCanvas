#ifndef UINODEITEM_H
#define UINODEITEM_H

#include <QObject>
#include <QString>
#include <QVector>

class UINodeItem : public QObject
{
    Q_OBJECT

public:
    explicit UINodeItem(UINodeItem* parentItem = nullptr);
    ~UINodeItem();

    // 添加子节点
    void appendChildNode(UINodeItem* nodeItem);
    // 刪除子节点
    void removeChildNode(const QString& nodeName);
    // 获取子节点
    UINodeItem* getChildNode(int index);
    UINodeItem* getChildNode(const QString& nodeName);

    // 设置/获取父节点
    void setParentNode(UINodeItem* node);
    UINodeItem* getParentNode(void);

    // 设置/获取节点名字
    void setName(const QString& name);
    QString getName(void);

    // 设置/获取是否为画布的节点
    void setCanvasNodeVisible(bool isNode);
    bool isCanvasNode(void);

    // 设置/获取节点类型
    void setNodeType(int type);
    int getNodeType(void);

    // 获取当前的索引
    int getCurrentIndex(void);
    int getCurrentIndex(UINodeItem* node);

    // 获取子元素的数量
    int getSubNodeCount(void);

private:
    QString m_cDisplayName;             // 显示的名字
    bool m_isCanvasNode = false;        // 是否能更改名字
    int m_nNodeType = 0;                // 节点类型

    QVector<UINodeItem*> m_childItems;      // 子节点列表
    UINodeItem* m_pParentItem = nullptr;    // 父节点
};

#endif
