#include "UICustomTimeEditControl.h"

UICustomTimeEditControl::UICustomTimeEditControl(QWidget* parent)
    :UICustomCombineControlBase(parent)
{
    m_pTimeEdit = new UICustomTimeEdit;
    m_pTimeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_pMainLayout->addWidget(m_pTimeEdit);
}

UICustomTimeEditControl::~UICustomTimeEditControl()
{

}

void UICustomTimeEditControl::setCurrentTime(const QTime& time)
{
    m_pTimeEdit->setTime(time);
}

QTime UICustomTimeEditControl::getCurrentTime(void)
{
    return m_pTimeEdit->time();
}

