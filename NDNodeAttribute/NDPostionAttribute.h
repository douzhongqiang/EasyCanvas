#ifndef NDPOSTIONATTRIBUTE_H
#define NDPOSTIONATTRIBUTE_H

#include "NDAttributeBase.h"
#include <QPoint>
class NDPostionAttribute : public NDAttributeBase
{
    Q_OBJECT
public:
    NDPostionAttribute(NDNodeBase *parentNode = nullptr);
    ~NDPostionAttribute();

    AttributeType Type(void) const override{
        return t_postion2d;
    }

    // 设置/获取 当前值
    void setCurrentValue(const QPoint& value);
    QPoint getCurrentValue(void);

private:
    QPoint m_value;

signals:
    void valueChanged(const QPoint& value);
};

#endif
