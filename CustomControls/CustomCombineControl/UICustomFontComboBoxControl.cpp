#include "UICustomFontComboBoxControl.h"
#include <QFontComboBox>

UICustomFontComboBoxControl::UICustomFontComboBoxControl(QWidget* parent)
    :UICustomCombineControlBase(parent)
{
    m_pComboBox = new QFontComboBox;
    m_pComboBox->setEditable(false);
    m_pComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_pMainLayout->addWidget(m_pComboBox);

    this->setWidth(400);
    this->setHeight(30);
}

UICustomFontComboBoxControl::~UICustomFontComboBoxControl()
{

}
