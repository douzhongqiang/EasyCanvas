#include "UICustomLeftWidget.h"
#include <QVBoxLayout>
#include "UICustomLeftItemWidget.h"
#include "UICustomCore/CustomStyleConfig.h"

UICutomLeftWidget::UICutomLeftWidget(QWidget* parent)
    :CustomWidget(parent)
{
    this->setMinimumWidth(250);
    this->setMaximumWidth(300);
    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->setMargin(4);
    m_pMainLayout->setSpacing(2);
    m_pMainLayout->addStretch();

    g_StyleConfig->setCurrentStyle(this, "LeftBar");
}

UICutomLeftWidget::~UICutomLeftWidget()
{

}

void UICutomLeftWidget::addItem(const QString& name, const QString& iconPath)
{
    UICustomLeftItemWidget* item = new UICustomLeftItemWidget;
    QObject::connect(item, &UICustomLeftItemWidget::clickedItem, this, &UICutomLeftWidget::onClickedItem);
    item->setText(name);

    if (!iconPath.isEmpty())
        item->setIcon(iconPath);

    m_pMainLayout->insertWidget(m_items.count(), item);
    m_items << item;
}

void UICutomLeftWidget::cleanAllSelected(void)
{
    foreach(auto item , m_items)
        item->setSelected(false);
}

void UICutomLeftWidget::onClickedItem(void)
{
    UICustomLeftItemWidget* itemWidget = qobject_cast<UICustomLeftItemWidget*>(sender());
    if (itemWidget == nullptr)
        return;

    cleanAllSelected();
    itemWidget->setSelected(true);

    // 查找点击的Item的索引
    int index = 0;
    bool hasFinded = false;
    for (auto iter = m_items.begin(); iter != m_items.end(); ++iter)
    {
        if (itemWidget == *iter)
        {
            hasFinded = true;
            break;
        }

        index++;
    }

    // 发送点击信号
    if (hasFinded)
        emit clickedItem(index);
}

void UICutomLeftWidget::setCurrentSelected(int index)
{
    m_items[index]->setSelected(true);
}
