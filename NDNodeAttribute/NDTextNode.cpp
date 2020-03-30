#include "NDTextNode.h"
#include "NDBoolAttribute.h"
#include "NDRealAttribute.h"
#include "NDColorAttribute.h"
#include "NDIntAttribute.h"
#include "NDStringAttribute.h"

NDTextNode::NDTextNode(QObject* parent)
    :NDNodeBase(t_textNode, parent)
{
    initAttribute();
}

NDTextNode::~NDTextNode()
{

}

void NDTextNode::initAttribute(void)
{
    // 文本位置信息
    m_pPostionXAttr = new NDRealAttribute(this);
    m_pPostionXAttr->setName("XPostion");
    m_pPostionXAttr->setDisplayName(tr("X Postion: "));
    //this->addAttribute(m_pPostionXAttr);

    m_pPostionYAttr = new NDRealAttribute(this);
    m_pPostionYAttr->setName("YPostion");
    m_pPostionYAttr->setDisplayName(tr("Y Postion: "));
    //this->addAttribute(m_pPostionYAttr);

    // 文本内容
    m_pTextAttr = new NDStringAttribute(this);
    m_pTextAttr->setName("text");
    m_pTextAttr->setDisplayName(tr("Text Content: "));
    //this->addAttribute(m_pTextAttr);

    // 文本颜色
    m_pTextColorAttr = new NDColorAttribute(this);
    m_pTextColorAttr->setName("textColor");
    m_pTextColorAttr->setDisplayName(tr("Text Color: "));
    //this->addAttribute(m_pTextColorAttr);

//    NDStringAttribute* m_pTextAttr = nullptr;
//    NDColorAttribute* m_pTextColorAttr = nullptr;
//    NDIntAttribute* m_pFontSizeAttr = nullptr;
//    NDStringAttribute* m_pFontFamilyAttr = nullptr;

//    // 轮廓线相关
//    NDBoolAttribute* m_pOutlineSwitchAttr = nullptr;
//    NDIntAttribute* m_pOutlineWidthAttr = nullptr;
//    NDColorAttribute* m_pOutlineColorAttr = nullptr;
}

NDRealAttribute* NDTextNode::getXPostionAttr(void)
{
    return m_pPostionXAttr;
}

NDRealAttribute* NDTextNode::getYPostionAttr(void)
{
    return m_pPostionYAttr;
}

NDStringAttribute* NDTextNode::getTextAttr(void)
{
    return m_pTextAttr;
}

NDColorAttribute* NDTextNode::getTextColorAttr(void)
{
    return m_pTextColorAttr;
}
