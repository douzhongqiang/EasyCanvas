#ifndef UINODETREEVIEW_H
#define UINODETREEVIEW_H

#include <QTreeView>
#include <QWidget>
#include "easycanvascore_global.h"

class UINodeTreeViewModel;
class UINodeTreeDelegate;
class UINodeSortFilterProxyModel;
class EASYCANVASCORESHARED_EXPORT UINodeTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit UINodeTreeView(QWidget* parent = nullptr);
    ~UINodeTreeView();

    void setFilterString(const QString& filterString);

    void updateView(void);

private:
    UINodeTreeViewModel* m_pModel = nullptr;
    UINodeTreeDelegate* m_pDelegate = nullptr;
    UINodeSortFilterProxyModel* m_pSortFilterModel = nullptr;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    // 响应节点被选中
    void onItemSelectedChanged(void);
    // 节点的选择发生改变
    void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

#endif
