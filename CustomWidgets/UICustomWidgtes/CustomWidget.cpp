#include "CustomWidget.h"
#include "UICustomCore/CustomStyleConfig.h"
#include <QStyleOption>
#include <QPainter>

CustomWidget::CustomWidget(QWidget* parent)
    :QWidget(parent)
{
    g_StyleConfig->setCurrentStyle(this, "DefaultWidget");
}

CustomWidget::~CustomWidget()
{

}

void CustomWidget::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    customPaint(&p);
    return QWidget::paintEvent(event);
}

void CustomWidget::customPaint(QPainter* painter)
{

}
