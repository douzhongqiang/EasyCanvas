#include "UICustomColorViewWidget.h"
#include "UICustomCore/CustomStyleConfig.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>

UICustomColorViewWidget::UICustomColorViewWidget(QWidget* parent)
    :CustomWidget(parent)
    , m_selectedColor(g_StyleConfig->getHighLightColor())
{
    g_StyleConfig->setCurrentStyle(this, "Transparent");
    this->setAcceptDrops(false);
}

UICustomColorViewWidget::~UICustomColorViewWidget()
{

}

void UICustomColorViewWidget::customPaint(QPainter* painter)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    if (m_isSelected)
    {
        QPen pen;
        pen.setColor(m_selectedColor);
        pen.setWidth(2);
        painter->setPen(pen);
    }
    else
        painter->setPen(Qt::NoPen);

    painter->setBrush(QBrush(m_currentColor));
    painter->drawRoundedRect(this->rect().adjusted(2, 2, -2, -2), 4, 4);

    painter->restore();
}

QSize UICustomColorViewWidget::sizeHint() const
{
    QSize size(80, 30);
    return size;
}

void UICustomColorViewWidget::mousePressEvent(QMouseEvent* event)
{
    if (m_isDragVisible)
    {
        QDrag *drag = new QDrag(this);
        QMimeData *mineData = new QMimeData;
        mineData->setColorData(m_currentColor);
        drag->setMimeData(mineData);

        drag->exec();
    }
    else
    {
        event->accept();
        emit mousePressed();
    }
}

void UICustomColorViewWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasColor())
        event->acceptProposedAction();

    emit dragEnterSignals();
}

void UICustomColorViewWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    emit dragReleaseSignals();
}

void UICustomColorViewWidget::dragMoveEvent(QDragMoveEvent *event)
{

}

void UICustomColorViewWidget::dropEvent(QDropEvent *event)
{
    if (!event->mimeData()->hasColor())
        return;

    // 设置当前颜色
    QColor color = qvariant_cast<QColor>(event->mimeData()->colorData());
    this->setCurrentColor(color);

    emit dropSignal();
}

void UICustomColorViewWidget::setCurrentColor(const QColor& color)
{
    m_currentColor = color;
    this->update();
}

const QColor& UICustomColorViewWidget::getCurrentColor(void)
{
    return m_currentColor;
}

void UICustomColorViewWidget::setDragVisible(bool isVisible)
{
    m_isDragVisible = isVisible;
}

void UICustomColorViewWidget::setSelected(bool isSelected)
{
    m_isSelected = isSelected;
    this->update();
}

bool UICustomColorViewWidget::getSelected(void)
{
    return m_isSelected;
}
