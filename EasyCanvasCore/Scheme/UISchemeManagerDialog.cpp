#include "UISchemeManagerDialog.h"
#include "UISchemeManagerWidget.h"
#include "CanvasSchemeManager.h"
#include "UICanvas/UICanvasItemManager.h"
#include <QVBoxLayout>

UISchemeManagerDialog::UISchemeManagerDialog(QWidget* parent)
    :CustomDialog(parent)
{
    initUi();

    // 同步数据
    m_pSchemeWidget->syncFromData();
    // 改变大小
    this->resize(800, 600);
    this->setWindowTitle(tr("Open Scheme"));
}

UISchemeManagerDialog::~UISchemeManagerDialog()
{

}

void UISchemeManagerDialog::initUi(void)
{
    // 设置Widget
    QWidget* contentWidget = new QWidget;
    this->setContentWidget(contentWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(contentWidget);
    // 添加方案管理Widget
    m_pSchemeWidget = new UISchemeManagerWidget;
    mainLayout->addWidget(m_pSchemeWidget);

    // 添加按钮
    QWidget* pButtonWidget = new QWidget;
    mainLayout->addWidget(pButtonWidget);
    QHBoxLayout* buttomLayout = new QHBoxLayout(pButtonWidget);
    buttomLayout->setMargin(0);
    buttomLayout->setSpacing(4);
    buttomLayout->addStretch();
    // 添加删除按钮
    QPushButton* pDeleteButton = new QPushButton(tr("Delete"));
    QObject::connect(pDeleteButton, &QPushButton::clicked, this, &UISchemeManagerDialog::onClickedDeleteButton);
    pDeleteButton->setFixedWidth(120);
    buttomLayout->addWidget(pDeleteButton);
    // 添加打开按钮
    QPushButton* pOpenButton = new QPushButton(tr("Open"));
    QObject::connect(pOpenButton, &QPushButton::clicked, this, &UISchemeManagerDialog::onClickedOpenButton);
    pOpenButton->setFixedWidth(120);
    buttomLayout->addWidget(pOpenButton);
}

void UISchemeManagerDialog::onClickedDeleteButton(void)
{
    // 获取当前选择的方案名称
    QString schemeName = m_pSchemeWidget->getCurrentSelectedSchemeName();
    if (schemeName.isEmpty())
        return;

    // 从数据库中删除
    g_CanvasSchemeManager->deleteScheme(schemeName);

    // 刷新数据显示
    m_pSchemeWidget->syncFromData();
}

void UISchemeManagerDialog::onClickedOpenButton(void)
{
    // 获取当前选择的方案名称
    QString schemeName = m_pSchemeWidget->getCurrentSelectedSchemeName();
    if (schemeName.isEmpty())
        return;

    this->close();

    // 打开方案
    g_currentCanvasManager->cleanAll();
    g_CanvasSchemeManager->setCurrentSchemeName(schemeName);
    g_CanvasSchemeManager->openScheme();
}
