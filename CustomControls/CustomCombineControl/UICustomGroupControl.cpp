#include "UICustomGroupControl.h"
#include "UICustomCore/CustomStyleConfig.h"
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

UICustomGroupTitleItem::UICustomGroupTitleItem(QWidget* parent)
    :CustomWidget(parent)
{
    g_StyleConfig->setCurrentStyle(this, "GroupTitle");
    this->setMinimumHeight(30);
}

UICustomGroupTitleItem::~UICustomGroupTitleItem()
{

}

void UICustomGroupTitleItem::mousePressEvent(QMouseEvent* event)
{
    m_isExpanding = !m_isExpanding;
    this->update();
    event->accept();
    emit clickedTitle(m_isExpanding);
}

void UICustomGroupTitleItem::customPaint(QPainter* painter)
{
    QTextOption o;
    o.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QPen pen;
    pen.setColor(QColor(230, 230, 230));
    painter->setPen(pen);

    int leftSpacing = 4;
    int drawMargin = 2;
    int expandWidth = 12;

    // 绘制展开
    qreal topHeight = (this->rect().height() - expandWidth) / 2.0;
    QRectF spandRect(leftSpacing + drawMargin, topHeight, expandWidth, expandWidth);
    painter->drawRect(spandRect);
    QLineF line1(spandRect.x(), spandRect.center().y(), spandRect.right(), spandRect.center().y());
    QLineF line2(spandRect.center().x(), spandRect.top(), spandRect.center().x(), spandRect.bottom());
    painter->drawLine(line1);
    if (!m_isExpanding)
        painter->drawLine(line2);


    // 绘制Title
    qreal leftWidth = spandRect.x() + leftSpacing + drawMargin * 2 + expandWidth;
    QRectF rect(leftWidth, 0, this->width() - leftWidth, this->height());
    painter->drawText(rect, m_cTitleName, o);
}

void UICustomGroupTitleItem::setTitleText(const QString& titleNameString)
{
    m_cTitleName = titleNameString;
    this->update();
}

QSize UICustomGroupTitleItem::sizeHint() const
{
    QSize size(400, 30);
    return size;
}

// -----------------------------------------------------------------------------
// 组件内容
UICustomGroupContentItem::UICustomGroupContentItem(QWidget* parent)
    :CustomWidget(parent)
{
    m_pLayout = new QVBoxLayout(this);
    m_pLayout->setMargin(2);
    m_pLayout->setSpacing(2);

    g_StyleConfig->setCurrentStyle(this, "GroupContent");
}

UICustomGroupContentItem::~UICustomGroupContentItem()
{

}

void UICustomGroupContentItem::addWidget(QWidget* widget)
{
    m_pLayout->addWidget(widget);
}

void UICustomGroupContentItem::insertWidget(int index, QWidget* widget)
{
    m_pLayout->insertWidget(index, widget);
}

QSize UICustomGroupContentItem::sizeHint() const
{
    int count = m_pLayout->count();

    // 高度
    int height = 0;
    for (int i=0; i<count; ++i)
    {
        QWidget* pWidget = m_pLayout->itemAt(i)->widget();
        if (pWidget == nullptr)
            continue;

        height += pWidget->sizeHint().height();
    }

    height += 4 + 2 * count;
    QSize size(400, height);
    return size;
}

// -----------------------------------------------------------------------------
UICustomGroupControl::UICustomGroupControl(QWidget* parent)
    :CustomWidget(parent)
{
    m_pMainLayout = new QVBoxLayout(this);
    m_pMainLayout->setMargin(2);
    m_pMainLayout->setSpacing(0);

    m_pTitleItemWidget = new UICustomGroupTitleItem;
    m_pContentItemWidget = new UICustomGroupContentItem;

    m_pMainLayout->addWidget(m_pTitleItemWidget);
    m_pMainLayout->addWidget(m_pContentItemWidget);

    g_StyleConfig->setCurrentStyle(this, "GroupWidget");

    QObject::connect(m_pTitleItemWidget, &UICustomGroupTitleItem::clickedTitle, \
                     this, &UICustomGroupControl::onClickedGroupTitleItem);
}

UICustomGroupControl::~UICustomGroupControl()
{

}

void UICustomGroupControl::addContentWidget(QWidget* widget)
{
    m_pContentItemWidget->addWidget(widget);
}

void UICustomGroupControl::insertContentWidget(int index, QWidget* widget)
{
    m_pContentItemWidget->insertWidget(index, widget);
}

void UICustomGroupControl::setTitleText(const QString& string)
{
    m_pTitleItemWidget->setTitleText(string);
}

void UICustomGroupControl::onClickedGroupTitleItem(bool isExpanded)
{
    m_pContentItemWidget->setVisible(isExpanded);
}

QSize UICustomGroupControl::sizeHint() const
{
    int height = 4;
    height += m_pTitleItemWidget->sizeHint().height();

    if (m_pContentItemWidget->isVisible())
        height += m_pContentItemWidget->sizeHint().height();

    return QSize(400, height);
}
