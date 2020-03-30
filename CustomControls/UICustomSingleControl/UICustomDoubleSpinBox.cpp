#include "UICustomDoubleSpinBox.h"
#include <QKeyEvent>

UICustomDoubleSpinBox::UICustomDoubleSpinBox(QWidget* parent)
    :QDoubleSpinBox(parent)
{

}

UICustomDoubleSpinBox::~UICustomDoubleSpinBox()
{

}

QSize UICustomDoubleSpinBox::sizeHint() const
{
    QSize size(80, 30);
    return size;
}

void UICustomDoubleSpinBox::keyPressEvent(QKeyEvent* event)
{
    QDoubleSpinBox::keyPressEvent(event);
    event->accept();
}
