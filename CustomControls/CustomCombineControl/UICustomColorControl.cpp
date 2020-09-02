#include "UICustomColorControl.h"
#include "UICustomColorControl/UICustomColorWidget.h"
#include "UICustomColorControl/UICustonColorDialog.h"

UICustomColorControl::UICustomColorControl(QWidget* parent)
    :UICustomCombineControlBase(parent)
{
    m_pColorViewWidget = new UICustomColorViewWidget;
    m_pMainLayout->addWidget(m_pColorViewWidget);

    // 添加Slider
    m_pSlider = new QSlider(Qt::Horizontal);
    m_pSlider->setMinimum(0);
    m_pSlider->setMaximum(255);
    m_pSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pMainLayout->addWidget(m_pSlider);

    QObject::connect(m_pSlider, &QSlider::valueChanged, this, &UICustomColorControl::onSliderValueChanged);
    QObject::connect(m_pSlider, &QSlider::sliderPressed, this, &UICustomColorControl::onSliderPressed);
    QObject::connect(m_pSlider, &QSlider::sliderMoved, this, &UICustomColorControl::onSliderMoved);
    QObject::connect(m_pSlider, &QSlider::sliderReleased, this, &UICustomColorControl::onSliderReleased);

    QObject::connect(m_pColorViewWidget, &UICustomColorViewWidget::mousePressed, \
                     this, &UICustomColorControl::onColorViewClicked);

    this->setWidth(400);
    this->setHeight(30);
}

UICustomColorControl::~UICustomColorControl()
{

}

void UICustomColorControl::setCurrentColor(const QColor& color, bool cmd)
{
    if (color == m_pColorViewWidget->getCurrentColor())
        return;
    m_pColorViewWidget->setCurrentColor(color);

    int h,s,v;
    color.getHsv(&h, &s, &v);

    m_pSlider->blockSignals(true);
    m_pSlider->setValue(v);
    m_pSlider->blockSignals(false);

    // 发送颜色更改信号
    emit colorChanged(color, cmd);
}

const QColor& UICustomColorControl::getCurrentColor(void)
{
    return m_pColorViewWidget->getCurrentColor();
}

void UICustomColorControl::onSliderMoved(void)
{
    setColorByVValue(m_pSlider->value());
}

void UICustomColorControl::setColorByVValue(int vValue, bool cmd)
{
    QColor color = m_pColorViewWidget->getCurrentColor();

    int h,s,v;
    color.getHsv(&h, &s, &v);
    color.setHsv(h, s, vValue);

    if (color != m_pColorViewWidget->getCurrentColor() || cmd)
    {
        m_pColorViewWidget->setCurrentColor(color);
        emit colorDragChanged(color, cmd);
    }
}

void UICustomColorControl::onSliderPressed(void)
{
    m_tempColor = m_pColorViewWidget->getCurrentColor();
    setColorByVValue(m_pSlider->value());
}

void UICustomColorControl::onSliderReleased(void)
{
    int value = m_pSlider->value();

    setCurrentColor(m_tempColor);
    setColorByVValue(value, true);

    m_pSlider->blockSignals(true);
    m_pSlider->setValue(value);
    m_pSlider->blockSignals(false);
}

void UICustomColorControl::onSliderValueChanged(int value)
{
    setColorByVValue(value);
}

void UICustomColorControl::onColorViewClicked(void)
{
    UICustomColorDialog dialog;
    dialog.setCurrentColor(m_pColorViewWidget->getCurrentColor());
    if (UICustomColorDialog::Accepted != dialog.exec())
        return;

    QColor color = dialog.getCurrentColor();
    this->setCurrentColor(color, true);
}
