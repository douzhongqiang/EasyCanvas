#include "UIAttrTextControl.h"

UIAttrTextControl::UIAttrTextControl(NDAttributeBase* attribute, QWidget* parent)
    :UICustomLineEditControl(parent)
{
    m_tempString = "EasyCanvas";
    setAttribute(attribute);

    if (m_attribute == nullptr)
        return;

    // 根据属性类型，设置控件的按钮
    this->setToolButtonVisible(m_attribute->isShowButton());
    this->setToolButtonText(m_attribute->getButtonString());
    QObject::connect(this, &UIAttrTextControl::clickedToolButton, this, &UIAttrTextControl::onClickedToolButton);
}

UIAttrTextControl::~UIAttrTextControl()
{

}

void UIAttrTextControl::setAttribute(NDAttributeBase* attribute)
{
    if (attribute == nullptr || attribute->Type() != NDAttributeBase::t_string)
        return;

    m_attribute = qobject_cast<NDStringAttribute*>(attribute);
    if (m_attribute == nullptr)
        return;

    this->setText(m_attribute->getValue().toString());
    this->setTagText(m_attribute->getDisplayName());

    QObject::connect(m_attribute, &NDStringAttribute::valueChanged, this, &UIAttrTextControl::onTextAttrValueChanged);
    QObject::connect(this, &UIAttrTextControl::textChanged, this, &UIAttrTextControl::onControlTextChanged);
    QObject::connect(this, &UIAttrTextControl::editingFinished, this, &UIAttrTextControl::onControlEditFinished);
}

void UIAttrTextControl::onTextAttrValueChanged(const QVariant& value)
{
    this->blockSignals(true);
    this->setText(value.toString());
    this->blockSignals(false);
}

void UIAttrTextControl::onControlTextChanged(const QString& value)
{
    QObject::disconnect(m_attribute, &NDStringAttribute::valueChanged, this, &UIAttrTextControl::onTextAttrValueChanged);
    m_attribute->setValue(value);
    QObject::connect(m_attribute, &NDStringAttribute::valueChanged, this, &UIAttrTextControl::onTextAttrValueChanged);
}

void UIAttrTextControl::onControlEditFinished(void)
{
    QString text = this->getText();
    if (m_tempString == text)
        return;

    QObject::disconnect(m_attribute, &NDStringAttribute::valueChanged, this, &UIAttrTextControl::onTextAttrValueChanged);
    m_attribute->setValue(m_tempString);
    m_tempString = text;
    m_attribute->setValue(text, true);
    QObject::connect(m_attribute, &NDStringAttribute::valueChanged, this, &UIAttrTextControl::onTextAttrValueChanged);
}

void UIAttrTextControl::onClickedToolButton(void)
{
    auto func = m_attribute->getButtonFunction();

    if (func)
    {
        QString str = "";
        bool result = func(str);
        if (result)
            m_attribute->setValue(str, true);
    }
}
