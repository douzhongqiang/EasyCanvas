#ifndef NDSTRINGATTRIBUTE_H
#define NDSTRINGATTRIBUTE_H

#include "NDAttributeBase.h"
#include "ndnodeattribute_global.h"
#include <functional>
class NDNODEATTRIBUTESHARED_EXPORT NDStringAttribute : public NDAttributeBase
{
    Q_OBJECT

public:
    NDStringAttribute(NDNodeBase* parentNode = nullptr);
    ~NDStringAttribute();

    AttributeType Type(void) const override{
        return t_string;
    }

    // 设置/获取是否显示按钮
    void setShowButton(bool isShow);
    bool isShowButton(void);

    // 设置/获取按钮名字
    void setButtonString(const QString& buttonName);
    QString getButtonString(void);

    // 设置按钮的功能
    void setButtonFunction(std::function<bool(QString&)> func);
    std::function<bool(QString&)> getButtonFunction(void);

private:

    bool m_isShowButton = false;
    QString m_buttonString = "";
    std::function<bool(QString&)> m_buttonFunc;
};

#endif
