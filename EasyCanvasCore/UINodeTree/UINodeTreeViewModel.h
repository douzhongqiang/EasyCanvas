#ifndef UINODETREEVIEWMODEL_H
#define UINODETREEVIEWMODEL_H

#include <QAbstractItemModel>
#include <QTreeView>

class UICanvasItemManager;
class UINodeItem;
class UINodeTreeView;
class UINodeTreeViewModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit UINodeTreeViewModel(QObject* parent = nullptr);
    ~UINodeTreeViewModel() override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    // 更新数据节点
    void updateAllNodes(void);

    // 设置当前的TreeView
    void setCurrentTreeView(UINodeTreeView* pTreeView);

    // 获取一个索引
    QModelIndex getIndexByName(const QString nodeName);

    void refrush(void);

private:
    UICanvasItemManager* m_pCanvasItemData = nullptr;
    UINodeItem* m_pRootNodeItem = nullptr;

    UINodeTreeView* m_pTreeView = nullptr;

private slots:
    void onAddedNodeItem(int nodeType, const QString& name);
    void onDeleteNodeItem(int nodeType, const QString& name);
    void onChangeNodeItemName(int nodeType, const QString& srcName, const QString& destName);
};

#endif
