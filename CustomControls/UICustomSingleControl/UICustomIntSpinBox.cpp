#include "UICustomIntSpinBox.h"
#include <QKeyEvent>

UICustomIntSpinBox::UICustomIntSpinBox(QWidget* parent)
    :QSpinBox(parent)
{

}

UICustomIntSpinBox::~UICustomIntSpinBox()
{

}

QSize UICustomIntSpinBox::sizeHint() const
{
    QSize size(80, 30);
    return size;
}

void UICustomIntSpinBox::keyPressEvent(QKeyEvent* event)
{
    QSpinBox::keyPressEvent(event);
    event->accept();
}
