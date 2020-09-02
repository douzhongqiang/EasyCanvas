#include "UISchemeManagerWidget.h"
#include "CanvasSchemeManager.h"
#include "SchemeDataInfo.h"
#include <QStandardItem>
#include <QItemSelectionModel>
#include <QPainter>
#include <QLineEdit>

UISchemeManagerWidget::UISchemeManagerWidget(QWidget* parent)
    :QListView(parent)
{
    // 设置Model
    m_pModel = new SchemeManagerModel(this);
    this->setModel(m_pModel);

    // 设置Delegate
    m_delegate = new SchemeManagerDelegate(this);
    this->setItemDelegate(m_delegate);

    this->setViewMode(QListView::IconMode);
    this->setResizeMode(QListView::Adjust);
    this->setSelectionMode(QListView::SingleSelection);
    this->setSpacing(4);
}

UISchemeManagerWidget::~UISchemeManagerWidget()
{

}

void UISchemeManagerWidget::syncFromData(void)
{
    // 更新显示
    this->clearSelection();
    this->doItemsLayout();
}

QString UISchemeManagerWidget::getCurrentSelectedSchemeName(void)
{
    QModelIndexList indexs = this->selectionModel()->selectedIndexes();
    if (indexs.isEmpty())
        return "";

    return m_pModel->data(indexs.at(0), Qt::DisplayRole).toString();
}

// ========================== Model ===================================
SchemeManagerModel::SchemeManagerModel(QObject* parent)
    :QAbstractListModel(parent)
{

}

SchemeManagerModel::~SchemeManagerModel()
{

}

int SchemeManagerModel::rowCount(const QModelIndex &parent) const
{
    const std::set<SchemeDataInfo::SchemeInfo>& infos = g_CanvasSchemeManager->getSchemeInfoList();
    return infos.size();
}

QVariant SchemeManagerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        int rowIndex = index.row();
        const std::set<SchemeDataInfo::SchemeInfo>& infos = g_CanvasSchemeManager->getSchemeInfoList();
        if (infos.size() <= rowIndex)
            return QVariant();

        // 获取名字
        return g_CanvasSchemeManager->getSchemeName(rowIndex);
    }

    return QVariant();
}

bool SchemeManagerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    int rowIndex = index.row();
    QString schemeName = g_CanvasSchemeManager->getSchemeName(rowIndex);
    g_CanvasSchemeManager->changedSchemeName(schemeName, value.toString());
    return true;
}

Qt::ItemFlags SchemeManagerModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return QAbstractListModel::flags(index);

    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

// ========================== Delegate ===================================
SchemeManagerDelegate::SchemeManagerDelegate(QObject* parent)
    :QStyledItemDelegate(parent)
{

}

SchemeManagerDelegate::~SchemeManagerDelegate()
{

}


void SchemeManagerDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

//    QString name = g_CanvasSchemeManager->getSchemeName(index.row());
//    painter->drawText(option.)
    //painter->fillRect(option.rect, QBrush(QColor(255, 0, 0)));

    // 获取信息数据
    SchemeDataInfo::SchemeInfo info = g_CanvasSchemeManager->getSchemeInfo(index.row());

    painter->setRenderHint(QPainter::Antialiasing);
    // 绘制边框
    painter->setPen(QColor(253, 135, 50));
    if (option.state & QStyle::State_Selected)
        painter->setBrush(QBrush(QColor(253, 135, 50)));
    else if (option.state & QStyle::State_MouseOver)
        painter->setBrush(QBrush(QColor(253, 135, 50, 100)));
    else {
        painter->setBrush(QBrush(QColor(100, 100, 100)));
    }
    painter->drawRoundedRect(option.rect, 4, 4);

    // 绘制图标
    int fontHeight = painter->fontMetrics().height() + 5;
    QRect iconRect = option.rect.adjusted(2, 3, -2, -fontHeight);
    if (!info.image.isNull())
    {
        qreal rato = info.image.width() * 1.0 / info.image.height();
        int height = qMin((int)(iconRect.width() / rato), iconRect.height());
        int width = rato * height;
        QRect imageRect(iconRect.x() + (iconRect.width() - width) / 2, \
                        iconRect.y() + (iconRect.height() - height) / 2, \
                        width, height);

        painter->drawImage(imageRect, info.image);
    }

    // 绘制文本
    QString schemeName = info.schemeName;
    painter->setPen(QColor(255, 255, 255));
    QTextOption o;
    o.setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    QRect textRect = option.rect.adjusted(0, 0, 0, -2);
    painter->drawText(textRect, schemeName, o);
}

QWidget *SchemeManagerDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit* lineEdit = new QLineEdit(parent);

    int rowIndex = index.row();
    QString schemeName = g_CanvasSchemeManager->getSchemeName(rowIndex);
    lineEdit->setText(schemeName);

    return lineEdit;
}

void SchemeManagerDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit* pLineEdit = qobject_cast<QLineEdit*>(editor);
    if (pLineEdit == nullptr)
        return;

    int rowIndex = index.row();
    QString schemeName = g_CanvasSchemeManager->getSchemeName(rowIndex);
    pLineEdit->setText(schemeName);
}

void SchemeManagerDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
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

QSize SchemeManagerDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(320, 180);
}

void SchemeManagerDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect nRect = option.rect;

    QFontMetrics fontMetrics(option.font);
    nRect.adjust(0, nRect.height() - fontMetrics.height(), 0, 2);
    editor->setGeometry(nRect);
}
