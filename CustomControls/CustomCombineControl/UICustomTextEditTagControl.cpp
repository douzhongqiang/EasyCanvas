#include "UICustomTextEditTagControl.h"
#include <QLabel>

UICustomTextEditTagControl::UICustomTextEditTagControl(QWidget* parent)
    :UICustomCombineControlBase(parent)
{
    m_pTextEdit = new UICustomTextEdit;
    m_pTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    m_pMainLayout->addWidget(m_pTextEdit);
}

UICustomTextEditTagControl::~UICustomTextEditTagControl()
{

}

QString UICustomTextEditTagControl::getCurrentText(void)
{
    return m_pTextEdit->toPlainText();
}

void UICustomTextEditTagControl::setCurrentText(const QString& text)
{
    m_pTextEdit->clear();
    m_pTextEdit->setPlainText(text);
}
