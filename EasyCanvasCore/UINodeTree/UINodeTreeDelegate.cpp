#include "UINodeTreeDelegate.h"
#include "UINodeItem.h"
#include "UINodeSortFilterProxyModel.h"
#include <QLineEdit>

UINodeTreeDelegate::UINodeTreeDelegate(QObject* parent)
    :QStyledItemDelegate(parent)
{

}

UINodeTreeDelegate::~UINodeTreeDelegate()
{

}

void UINodeTreeDelegate::setProxyModel(UINodeSortFilterProxyModel* model)
{
    m_pModel = model;
}

QWidget *UINodeTreeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid())
        return QStyledItemDelegate::createEditor(parent, option, index);

    QModelIndex srcIndex = m_pModel->mapToSource(index);
    UINodeItem* node = static_cast<UINodeItem*>(srcIndex.internalPointer());
    bool isCanChangedName = node->isCanvasNode();
    if (!isCanChangedName)
        return QStyledItemDelegate::createEditor(parent, option, index);

    QLineEdit* pLineEdit = new QLineEdit(parent);
    pLineEdit->setText(node->getName());
    return pLineEdit;
}

void UINodeTreeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit* pLineEdit = qobject_cast<QLineEdit*>(editor);
    if (pLineEdit == nullptr)
        return;

    QModelIndex srcIndex = m_pModel->mapToSource(index);
    UINodeItem* node = static_cast<UINodeItem*>(srcIndex.internalPointer());
    pLineEdit->setText(node->getName());
}

void UINodeTreeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (model == nullptr || !index.isValid())
        return;

    QLineEdit* pLineEdit = qobject_cast<QLineEdit*>(editor);
    if (pLineEdit == nullptr)
        return;

    QString name = pLineEdit->text();
    if (name.isEmpty())
        return;

    model->setData(index, name, Qt::EditRole);
}

QSize UINodeTreeDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid())
        return QStyledItemDelegate::sizeHint(option, index);

    QSize size;
    size.setWidth(option.decorationSize.width());
    size.setHeight(30);
    return size;
}
