#include "UICustomLineEditControl.h"

UICustomLineEditControl::UICustomLineEditControl(QWidget* parent)
    :UICustomCombineControlBase(parent)
{
    m_pLineEdit = new UICustomLineEdit;
    m_pMainLayout->addWidget(m_pLineEdit);
    m_pLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_pToolButton = new QToolButton;
    m_pMainLayout->addWidget(m_pToolButton);

    // 转发信后
    QObject::connect(m_pLineEdit, &QLineEdit::textChanged, this, &UICustomLineEditControl::textChanged);
    QObject::connect(m_pLineEdit, &QLineEdit::editingFinished, this, &UICustomLineEditControl::editingFinished);
    QObject::connect(m_pToolButton, &QToolButton::clicked, this, &UICustomLineEditControl::clickedToolButton);

    this->setWidth(400);
    this->setHeight(30);
}

UICustomLineEditControl::~UICustomLineEditControl()
{

}

// 设置按钮相关
void UICustomLineEditControl::setToolButtonVisible(bool isVisible)
{
    m_pToolButton->setVisible(isVisible);
}

void UICustomLineEditControl::setToolButtonText(const QString& text)
{
    m_pToolButton->setText(text);
}

void UICustomLineEditControl::setToolButtonIcon(const QIcon& icon)
{
    m_pToolButton->setIcon(icon);
}

// 设置文本
void UICustomLineEditControl::setText(const QString& string)
{
    m_pLineEdit->setText(string);
}

QString UICustomLineEditControl::getText(void)
{
    return m_pLineEdit->text();
}
