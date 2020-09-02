#ifndef UINODETREEDELEGATE_H
#define UINODETREEDELEGATE_H

#include <QStyledItemDelegate>

class UINodeSortFilterProxyModel;
class UINodeTreeDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit UINodeTreeDelegate(QObject* parent = nullptr);
    ~UINodeTreeDelegate();

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setProxyModel(UINodeSortFilterProxyModel* model);

private:
    UINodeSortFilterProxyModel* m_pModel = nullptr;
};

#endif
