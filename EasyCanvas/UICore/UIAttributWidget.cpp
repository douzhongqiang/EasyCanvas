#include "UIAttributWidget.h"
#include "NDNodeBase.h"
#include "UINodeAttrControl.h"

UIAttributeWidget::UIAttributeWidget(QWidget* parent)
    :QWidget(parent)
{
    m_pMainLayout = new QVBoxLayout(this);
    this->setMinimumWidth(450);
}

UIAttributeWidget::~UIAttributeWidget()
{

}

// 设置当前显示的节点
void UIAttributeWidget::setCurrentAttrNode(NDNodeBase* node)
{
    if (m_pWidget)
    {
        m_pWidget->deleteLater();
        m_pWidget = nullptr;
    }

    QWidget* widget = UINodeAttrControl::createNodeWidget(node);
    if (widget == nullptr)
        return;

    m_pWidget = widget;

    // 设置Widget
    m_pMainLayout->addWidget(widget);
}
