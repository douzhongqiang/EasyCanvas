#include "UICustomLineEdit.h"
#include <QKeyEvent>

UICustomLineEdit::UICustomLineEdit(QWidget* parent)
    :QLineEdit(parent)
{

}

UICustomLineEdit::~UICustomLineEdit()
{

}

void UICustomLineEdit::keyPressEvent(QKeyEvent* event)
{
    QLineEdit::keyPressEvent(event);
    event->accept();
}
