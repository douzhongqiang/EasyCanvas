#include "UINodeTreeView.h"
#include "UINodeTreeViewModel.h"
#include "UINodeTreeDelegate.h"
#include "UICanvas/UICanvasItemManager.h"
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
}

UINodeTreeView::~UINodeTreeView()
{

}
