#include "UICustomLineEdit.h"
#include <QKeyEvent>
#include <QToolButton>
#include <QVBoxLayout>
#include <QPainter>
#include "UICustomCore/CustomStyleConfig.h"

UICustomLineEdit::UICustomLineEdit(QWidget* parent)
    :QLineEdit(parent)
{
    m_pButton = new UICustomCloseButton;
    m_pButton->setFixedSize(12, 12);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(m_pButton, 0, Qt::AlignRight | Qt::AlignVCenter);

    setCloseButtonEnable(false);

    // 关联信号和槽
    QObject::connect(this, &UICustomLineEdit::textChanged, this, &UICustomLineEdit::onTextChanged);
    QObject::connect(m_pButton, &UICustomCloseButton::clicked, this, &UICustomLineEdit::onCloseButtonClicked);
}

UICustomLineEdit::~UICustomLineEdit()
{

}

void UICustomLineEdit::keyPressEvent(QKeyEvent* event)
{
    QLineEdit::keyPressEvent(event);
    event->accept();
}

void UICustomLineEdit::setCloseButtonEnable(bool isEnabled)
{
    m_isToolButtonEnable = isEnabled;
    if (!isEnabled)
        m_pButton->setVisible(false);
}

void UICustomLineEdit::onTextChanged(const QString& textChanged)
{
    if (!m_isToolButtonEnable)
    {
        emit currentTextChanged(textChanged);
        return;
    }

    if (textChanged.isEmpty())
        m_pButton->setVisible(false);
    else
        m_pButton->setVisible(true);
    emit currentTextChanged(textChanged);
}

void UICustomLineEdit::onCloseButtonClicked(void)
{
    this->setText("");
}
// ==============================================================
UICustomCloseButton::UICustomCloseButton(QWidget* parent)
    :CustomWidget(parent)
{
    this->setMouseTracking(true);
    g_StyleConfig->setCurrentStyle(this, "TransparentBgWidget");
}

UICustomCloseButton::~UICustomCloseButton()
{

}

void UICustomCloseButton::customPaint(QPainter* painter)
{
    QColor penColor(200, 80, 80);
    if (m_isHoveredOn)
        penColor = QColor(200, 200, 200);

    // 设置画笔
    QPen pen;
    pen.setWidth(2);
    pen.setColor(penColor);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);

    // 绘制关闭
    QRect rect = this->rect();
    rect.adjust(2, 2, -2, -2);
    painter->drawLine(rect.topLeft(), rect.bottomRight());
    painter->drawLine(rect.topRight(), rect.bottomLeft());
}

void UICustomCloseButton::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
    CustomWidget::mousePressEvent(event);
}

void UICustomCloseButton::enterEvent(QEvent *event)
{
    m_isHoveredOn = true;
    this->setCursor(Qt::PointingHandCursor);
    this->update();
    CustomWidget::enterEvent(event);
}

void UICustomCloseButton::leaveEvent(QEvent *event)
{
    m_isHoveredOn = false;
    this->setCursor(Qt::ArrowCursor);
    this->update();
    CustomWidget::leaveEvent(event);
}
