#include "UINodeTreeWidget.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QAction>
#include "UINodeTreeView.h"
#include "UICustomCore/CustomStyleConfig.h"
#include "UICustomSingleControl/UICustomLineEdit.h"

UINodeTreeWidget::UINodeTreeWidget(QWidget* parent)
    :QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(2);
    mainLayout->setMargin(2);

    // 顶部搜索
    QWidget *pTopWidget = new QWidget;
    g_StyleConfig->setCurrentStyle(pTopWidget, "BottomLineWidget");
    QHBoxLayout* pTopLayout = new QHBoxLayout(pTopWidget);
    pTopLayout->setMargin(2);
    pTopLayout->setSpacing(0);
    QLabel* pHeadLabel = new QLabel(tr("Search Node:"));
    pTopLayout->addWidget(pHeadLabel);
    // 搜索框
    m_pLineEdit = new UICustomLineEdit;
    m_pLineEdit->setCloseButtonEnable(true);
    QObject::connect(m_pLineEdit, &UICustomLineEdit::currentTextChanged, this, &UINodeTreeWidget::onTextChanged);
    pTopLayout->addWidget(m_pLineEdit);
    mainLayout->addWidget(pTopWidget);

    // 添加节点树
    m_pTreeView = new UINodeTreeView;
    mainLayout->addWidget(m_pTreeView);
}

UINodeTreeWidget::~UINodeTreeWidget()
{

}

void UINodeTreeWidget::onTextChanged(const QString& text)
{
    m_pTreeView->setFilterString(text);
}
