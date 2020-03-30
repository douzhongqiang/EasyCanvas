#include "UICustomLeftItemWidget.h"
#include "UICustomCore/CustomStyleConfig.h"
#include <QPainter>

UICustomLeftItemWidget::UICustomLeftItemWidget(QWidget* parent)
    :CustomWidget(parent)
{
    g_StyleConfig->setCurrentStyle(this, "LeftBarItem");
}

UICustomLeftItemWidget::~UICustomLeftItemWidget()
{

}

void UICustomLeftItemWidget::customPaint(QPainter* painter)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    // 绘制选中矩形
    if (m_isSelected)
    {
        painter->save();
        painter->fillRect(QRect(0, 0, m_nLeftSeletedWidth, this->height()), \
                          QBrush(g_StyleConfig->getHighLightColor()));

        // 绘制三角形
        QPolygon polygon;
        polygon << QPoint(this->width() - 4 * m_nArrowWidth, this->rect().center().y()) \
                << QPoint(this->width(), this->rect().center().y() - 3 * m_nArrowWidth) \
                << QPoint(this->width(), this->rect().center().y() + 3 * m_nArrowWidth);

        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(g_StyleConfig->getHighLightColor()));
        painter->drawPolygon(polygon);
        painter->restore();
    }

    // 绘制图标
    int nIconSize = this->height() - 2 * 2;
    if (!m_pixmap.isNull())
        painter->drawPixmap(QRect(m_nLeftSeletedWidth + m_nInterval, 2, nIconSize, nIconSize), m_pixmap);

    // 绘制文字
    int xPt = nIconSize + m_nLeftSeletedWidth + m_nInterval * 2;
    QRect textRect(xPt, 0, this->width() - xPt, this->height());
    QTextOption o;
    o.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    painter->drawText(textRect, m_cString, o);

    painter->restore();
}

void UICustomLeftItemWidget::mousePressEvent(QMouseEvent* event)
{
    setSelected(true);
    this->update();

    // 发送点击信号
    emit clickedItem();

    return CustomWidget::mousePressEvent(event);
}

QSize UICustomLeftItemWidget::sizeHint(void) const
{
    return QSize(250, 50);
}

// 设置/获取是否被选中
void UICustomLeftItemWidget::setSelected(bool isSelected)
{
    m_isSelected = isSelected;
    if (m_isSelected)
        g_StyleConfig->setCurrentStyle(this, "LeftBarItemSelected", true);
    else
        g_StyleConfig->setCurrentStyle(this, "LeftBarItem", true);

    this->update();
}

bool UICustomLeftItemWidget::isSelected(void)
{
    return m_isSelected;
}

// 设置/获取字符串
void UICustomLeftItemWidget::setText(const QString& name)
{
    m_cString = name;
    this->update();
}

QString UICustomLeftItemWidget::getText(void)
{
    return m_cString;
}

void UICustomLeftItemWidget::setIcon(const QString& imageFile)
{
    m_pixmap.load(imageFile);
    this->update();
}
