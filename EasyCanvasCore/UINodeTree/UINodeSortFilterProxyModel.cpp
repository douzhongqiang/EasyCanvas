#include "UINodeSortFilterProxyModel.h"
#include "UINodeItem.h"

UINodeSortFilterProxyModel::UINodeSortFilterProxyModel(QObject* parent)
    :QSortFilterProxyModel(parent)
{
    this->setDynamicSortFilter(true);
    this->setRecursiveFilteringEnabled(true);
}

UINodeSortFilterProxyModel::~UINodeSortFilterProxyModel()
{

}

bool UINodeSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex & source_parent) const
{
    if (m_filterString.isEmpty())
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);

    QModelIndex index = this->sourceModel()->index(source_row, 0, source_parent);
    if (!index.isValid())
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);

    UINodeItem* pNodeItem = static_cast<UINodeItem*>(index.internalPointer());
    if (!pNodeItem || !pNodeItem->isCanvasNode())
        return false;

    QString name = this->sourceModel()->data(index).toString();
    if (name.indexOf(m_filterString, 0, Qt::CaseInsensitive) >= 0)
        return true;

    return false;
}

void UINodeSortFilterProxyModel::setFilterString(const QString& filterString)
{
    m_filterString = filterString;
    this->setFilterFixedString(m_filterString);
}
