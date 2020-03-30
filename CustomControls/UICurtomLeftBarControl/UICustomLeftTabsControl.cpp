#include "UICustomLefTabsControl.h"
#include "UICustomLeftWidget.h"
#include <QSplitter>
#include <QVBoxLayout>

UICustomLeftTabsControl::UICustomLeftTabsControl(QWidget* parent)
    :CustomWidget(parent)
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    mainLayout->addWidget(splitter);

    // 添加 左侧工具条
    m_pLeftWidget = new UICutomLeftWidget;
    QObject::connect(m_pLeftWidget, &UICutomLeftWidget::clickedItem, \
                     this, &UICustomLeftTabsControl::onClickedItem);
    splitter->addWidget(m_pLeftWidget);

    // 添加右侧主要部分
    m_pStackWidget = new QStackedWidget;
    splitter->addWidget(m_pStackWidget);

    // 设置 Handle Hover Attribute
    QSplitterHandle* handle = splitter->handle(1);
    if (handle != nullptr)
        handle->setAttribute(Qt::WA_Hover, true);
}

UICustomLeftTabsControl::~UICustomLeftTabsControl()
{

}

void UICustomLeftTabsControl::addWidget(QWidget* widget, const QString& name, const QString& iconPath)
{
    m_pLeftWidget->addItem(name, iconPath);
    m_pStackWidget->addWidget(widget);
}

void UICustomLeftTabsControl::setCurrentSelectedItem(int index)
{
    m_pLeftWidget->setCurrentSelected(index);
}

void UICustomLeftTabsControl::onClickedItem(int index)
{
    m_pStackWidget->setCurrentIndex(index);
}
