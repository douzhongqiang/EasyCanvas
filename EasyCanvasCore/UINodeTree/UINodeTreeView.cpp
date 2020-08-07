#include "UINodeTreeView.h"
#include "UINodeTreeViewModel.h"
#include "UINodeTreeDelegate.h"
#include "UICanvas/UICanvasItemManager.h"
#include "UICanvas/UICanvasView.h"
#include "NDNodeBase.h"
#include <QHeaderView>

UINodeTreeView::UINodeTreeView(QWidget* parent)
    :QTreeView(parent)
{
    // 设置model
    m_pModel = new UINodeTreeViewModel(this);
    m_pModel->setCurrentTreeView(this);
    this->setModel(m_pModel);

    // 设置Delegate
    m_pDelegate = new UINodeTreeDelegate(this);
    this->setItemDelegate(m_pDelegate);

    this->expandAll();
    this->header()->setVisible(false);
    this->setSelectionMode(QTreeView::ExtendedSelection);

    // 初始化选择
    UICanvasView* pCanvasView = g_currentCanvasManager->getCurrentCanvasView();
    QObject::connect(pCanvasView, &UICanvasView::itemSelectedChanged, this, &UINodeTreeView::onItemSelectedChanged);
    QItemSelectionModel* pSelectonModel = this->selectionModel();
    QObject::connect(pSelectonModel, &QItemSelectionModel::selectionChanged, this, &UINodeTreeView::onSelectionChanged);
}

UINodeTreeView::~UINodeTreeView()
{

}

void UINodeTreeView::onItemSelectedChanged(void)
{
    UICanvasView* pCanvasView = g_currentCanvasManager->getCurrentCanvasView();

    // 清除全部选中
    QItemSelectionModel* pSelectonModel = this->selectionModel();
    QObject::disconnect(pSelectonModel, &QItemSelectionModel::selectionChanged, this, &UINodeTreeView::onSelectionChanged);
    pSelectonModel->clear();

    // 获取被选中的节点
    QList<NDNodeBase*> nodes = pCanvasView->getCurrentSelectedNodes();
    foreach(NDNodeBase* pNode ,nodes)
    {
        // 获取节点的索引
        QModelIndex index = m_pModel->getIndexByName(pNode->getNodeName());

        // 选中
        pSelectonModel->select(index, QItemSelectionModel::Select);
    }

    QObject::connect(pSelectonModel, &QItemSelectionModel::selectionChanged, this, &UINodeTreeView::onSelectionChanged);
}

void UINodeTreeView::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    // 取消信号和槽函数的连接
    UICanvasView* pCanvasView = g_currentCanvasManager->getCurrentCanvasView();
    QObject::disconnect(pCanvasView, &UICanvasView::itemSelectedChanged, this, &UINodeTreeView::onItemSelectedChanged);

    QStringList nodeNames;

    QItemSelectionModel* pSelectonModel = this->selectionModel();
    QModelIndexList indexs = pSelectonModel->selectedIndexes();
    for (auto iter = indexs.begin(); iter != indexs.end(); ++iter)
    {
        nodeNames << iter->data().toString();
    }
    g_currentCanvasManager->setSelectedNodes(nodeNames);

    // 重新连接
    QObject::connect(pCanvasView, &UICanvasView::itemSelectedChanged, this, &UINodeTreeView::onItemSelectedChanged);
}

void UINodeTreeView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)
    {
        // 获取所有被选中的节点名字列表
        QStringList nodeNames;
        QItemSelectionModel* pSelectonModel = this->selectionModel();
        QModelIndexList indexs = pSelectonModel->selectedIndexes();
        for (auto iter = indexs.begin(); iter != indexs.end(); ++iter)
        {
            nodeNames << iter->data().toString();
        }

        // 删除节点
        g_currentCanvasManager->deleteCanvasItems(nodeNames);
    }
    return QTreeView::keyPressEvent(event);
}
