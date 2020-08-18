#ifndef UISCHEMEMANAGERWIDGET_H
#define UISCHEMEMANAGERWIDGET_H

#include <QListView>
#include <QWidget>
#include <QStandardItemModel>
#include <QAbstractListModel>
#include <QStyledItemDelegate>
#include "easycanvascore_global.h"

class SchemeManagerModel;
class SchemeManagerDelegate;
class EASYCANVASCORESHARED_EXPORT UISchemeManagerWidget : public QListView
{
    Q_OBJECT

public:
    explicit UISchemeManagerWidget(QWidget* parent = nullptr);
    ~UISchemeManagerWidget();

    // 同步数据
    void syncFromData(void);
    // 获取当前选择的方案
    QString getCurrentSelectedSchemeName(void);

private:
    SchemeManagerModel* m_pModel = nullptr;
    SchemeManagerDelegate* m_delegate = nullptr;
};

// ========================== Model ===================================
class SchemeManagerModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SchemeManagerModel(QObject* parent = nullptr);
    ~SchemeManagerModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
};

// ========================== Delegate ===================================
class SchemeManagerDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit SchemeManagerDelegate(QObject* parent = nullptr);
    ~SchemeManagerDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif
