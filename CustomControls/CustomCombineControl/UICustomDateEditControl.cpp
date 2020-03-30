#include "UICustomDateEditControl.h"

UICustomDateEditControl::UICustomDateEditControl(QWidget* parent)
    :UICustomCombineControlBase(parent)
{
    m_pDateEdit = new UICutomDateEdit;
    m_pDateEdit->setDisplayFormat("yyyy-MM-dd");

    m_pMainLayout->addWidget(m_pDateEdit);
}

UICustomDateEditControl::~UICustomDateEditControl()
{

}

void UICustomDateEditControl::setCurrentDate(const QDate& date)
{
    m_pDateEdit->setDate(date);
}

QDate UICustomDateEditControl::getCurrentDate(void)
{
    return m_pDateEdit->date();
}
