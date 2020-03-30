#include "UICustonColorDialog.h"

UICustomColorDialog::UICustomColorDialog(QWidget* parent)
    :CustomDialog(parent)
{
    init();

    QObject::connect(m_pColorWidget, &UICustomColorWidget::clickedOKButton, \
                     this, &UICustomColorDialog::accept);
    QObject::connect(m_pColorWidget, &UICustomColorWidget::clickedCannelButton, \
                     this, &UICustomColorDialog::reject);
}

UICustomColorDialog::~UICustomColorDialog()
{

}

void UICustomColorDialog::init(void)
{
    m_pColorWidget = new UICustomColorWidget;
    this->setContentWidget(m_pColorWidget);

    this->setWindowTitle(tr("Color Config"));
}

// 设置/获取颜色
void UICustomColorDialog::setCurrentColor(const QColor& color)
{
    m_pColorWidget->setCurrentColor(color);
}

QColor UICustomColorDialog::getCurrentColor(void)
{
    return m_pColorWidget->getCurrentColor();
}
