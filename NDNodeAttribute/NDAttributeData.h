#ifndef NDATTRIBUTEDATA_H
#define NDATTRIBUTEDATA_H

#include <QObject>
#include <QColor>
#include <QList>
#include <QString>
#include <QStringList>
#include "NDAttributeBase.h"
class NDAttributeData
{
public:
    NDAttributeData();
    virtual ~NDAttributeData();

    // 设置值
    void setValue(bool value);
    void setValue(int value);
    void setValue(qreal value);
    void setValue(const QString& value);
    void setValue(const QStringList& value);
    void setValue(const QColor& value);
    void setValue(const QPoint& value);

    // 获取值
    void getValue(bool& value);
    void getValue(int& value);
    void getValue(qreal& value);
    void getValue(QString& value);
    void getValue(QStringList& value);
    void getValue(QColor& value);
    void getValue(QPoint& value);

private:
    void* m_pData = nullptr;
};
#endif
