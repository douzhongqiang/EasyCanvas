#include "UICustomComboBox.h"
#include <QListView>

UICustomComboBox::UICustomComboBox(QWidget* parent)
    :QComboBox(parent)
{
    this->setView(new QListView(this));
}

UICustomComboBox::~UICustomComboBox()
{

}

QSize UICustomComboBox::sizeHint(void) const
{
    QSize size(80, 30);
    return size;
}
