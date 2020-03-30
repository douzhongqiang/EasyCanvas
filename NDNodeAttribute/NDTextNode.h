#ifndef NDTEXTNODE_H
#define NDTEXTNODE_H

#include "NDNodeBase.h"
#include "NDAttributeBase.h"
#include <QObject>

class NDStringAttribute;
class NDIntAttribute;
class NDRealAttribute;
class NDPostionAttribute;
class NDBoolAttribute;
class NDColorAttribute;

class NDTextNode : public NDNodeBase
{
public:
    NDTextNode(QObject* parent = nullptr);
    ~NDTextNode();

    // 获取属性
    NDRealAttribute* getXPostionAttr(void);
    NDRealAttribute* getYPostionAttr(void);
    NDStringAttribute* getTextAttr(void);
    NDColorAttribute* getTextColorAttr(void);

private:
    void initAttribute(void);

    NDRealAttribute* m_pPostionXAttr = nullptr;
    NDRealAttribute* m_pPostionYAttr = nullptr;
    NDStringAttribute* m_pTextAttr = nullptr;
    NDColorAttribute* m_pTextColorAttr = nullptr;
    NDIntAttribute* m_pFontSizeAttr = nullptr;
    NDStringAttribute* m_pFontFamilyAttr = nullptr;

    // 轮廓线相关
    NDBoolAttribute* m_pOutlineSwitchAttr = nullptr;
    NDIntAttribute* m_pOutlineWidthAttr = nullptr;
    NDColorAttribute* m_pOutlineColorAttr = nullptr;
};

#endif
