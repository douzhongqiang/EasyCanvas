#include "UICustomDateEdit.h"
#include "UICustomCalendarWidget.h"

UICutomDateEdit::UICutomDateEdit(QWidget* parent)
    :QDateEdit(parent)
{
    this->setCalendarPopup(true);
    this->setCalendarWidget(new UICustomCalendarWidget);
}

UICutomDateEdit::~UICutomDateEdit()
{

}
