#include "UICustomOrientationControl.h"
#include "UICustomCore/CustomStyleConfig.h"
#include <QPainter>
#include <QMouseEvent>
#include <QGridLayout>

UICustomOrientationItemWidget::UICustomOrientationItemWidget()
{
    m_color = QColor(80, 80, 80);
    this->setFixedSize(30, 30);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

UICustomOrientationItemWidget::~UICustomOrientationItemWidget()
{

}

void UICustomOrientationItemWidget::customPaint(QPainter* painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillRect(this->rect(), QBrush(m_color));

    QPen nPen;
    nPen.setColor(QColor(230, 230, 230));
    painter->setPen(nPen);

    if (m_isPressed)
        painter->drawEllipse(this->rect().adjusted(2, 2, -2, -2));
}

void UICustomOrientationItemWidget::mousePressEvent(QMouseEvent* event)
{
    m_isPressed = true;
    emit clickedItem();

    event->accept();
}

void UICustomOrientationItemWidget::setPressedStatus(bool status)
{
    m_isPressed = status;
    m_color = QColor(60, 60, 60);

    this->update();
}

QSize UICustomOrientationItemWidget::sizeHint() const
{
    return CustomWidget::sizeHint();

    QSize size(20, 20);

    return size;
}

// -----------------------------------------------------------------
UICustomOrientationWidget::UICustomOrientationWidget(QWidget* parent)
    :CustomWidget(parent)
{
    QGridLayout* layout = new QGridLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);

    for (int i=0; i<9; ++i)
    {
        UICustomOrientationItemWidget* itemWidget = new UICustomOrientationItemWidget;
        m_items << itemWidget;

        layout->addWidget(itemWidget, i / 3, i % 3);
        if (i + 1 % 3 == 0)
        {
            QSpacerItem *spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
            layout->addItem(spacerItem, i / 3, 3);
        }
        //itemWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QObject::connect(itemWidget, &UICustomOrientationItemWidget::clickedItem, \
                         this, &UICustomOrientationWidget::onClickedItem);
    }

}

UICustomOrientationWidget::~UICustomOrientationWidget()
{

}

void UICustomOrientationWidget::onClickedItem(void)
{
    UICustomOrientationItemWidget* pW = dynamic_cast<UICustomOrientationItemWidget*>(sender());
    if (pW == nullptr)
        return;

    cleanAllItemsStatus();
    pW->setPressedStatus(true);
}

void UICustomOrientationWidget::cleanAllItemsStatus(void)
{
    for (auto iter = m_items.begin(); iter != m_items.end(); ++iter)
        (*iter)->setPressedStatus(false);
}
// -----------------------------------------------------------------
UICustomOrientationControl::UICustomOrientationControl(QWidget* parent)
    :UICustomCombineControlBase(parent)
{
    m_pOrientationWidget = new UICustomOrientationWidget;
    m_pMainLayout->addWidget(m_pOrientationWidget);
    m_pOrientationWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //this->setFixedSize(200, 200);
}

UICustomOrientationControl::~UICustomOrientationControl()
{

}
