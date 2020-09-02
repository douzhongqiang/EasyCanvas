#ifndef UINODESORTFILTERPROXYMODEL_H
#define UINODESORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QObject>

class UINodeSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit UINodeSortFilterProxyModel(QObject* parent = nullptr);
    ~UINodeSortFilterProxyModel();

    void setFilterString(const QString& filterString);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const;

private:
    QString m_filterString;
};

#endif
