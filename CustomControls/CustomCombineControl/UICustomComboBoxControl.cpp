#include "UICustomComboBoxControl.h"

UICustomComboBoxControl::UICustomComboBoxControl(QWidget* parent)
    :UICustomCombineControlBase(parent)
{
    m_pCustomComboBox = new UICustomComboBox;
    m_pCustomComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_pMainLayout->addWidget(m_pCustomComboBox);

    QObject::connect(m_pCustomComboBox, SIGNAL(currentIndexChanged(int)), \
                     this, SIGNAL(currentIndexChanged(int)));
    QObject::connect(m_pCustomComboBox, SIGNAL(currentIndexChanged(const QString &)), \
                     this, SIGNAL(currentTextChanged(const QString&)));
}

UICustomComboBoxControl::~UICustomComboBoxControl()
{

}

// 添加元素
void UICustomComboBoxControl::addItem(const QString& item)
{
    m_pCustomComboBox->addItem(item);
}

void UICustomComboBoxControl::addItem(const QString& name, const QVariant& data)
{
    m_pCustomComboBox->addItem(name, data);
}

// 获取当前元素
QString UICustomComboBoxControl::getCurrentItem(void)
{
    return m_pCustomComboBox->currentText();
}

void UICustomComboBoxControl::setCurrentItem(const QString& text)
{
    m_pCustomComboBox->setCurrentText(text);
}

void UICustomComboBoxControl::setCurrentIndex(int index)
{
    m_pCustomComboBox->setCurrentIndex(index);
}

int UICustomComboBoxControl::getCurrentIndex(void)
{
    return m_pCustomComboBox->currentIndex();
}

QVariant UICustomComboBoxControl::getCurrentItemData(void)
{
    return m_pCustomComboBox->currentData();
}

int UICustomComboBoxControl::getItemCount(void)
{
    return m_pCustomComboBox->count();
}

void UICustomComboBoxControl::clearAllItems(void)
{
    m_pCustomComboBox->clear();
}
