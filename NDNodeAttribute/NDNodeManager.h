#ifndef NDNODEMANAGER_H
#define NDNODEMANAGER_H

#include <QObject>
#include <QList>
#include <QString>

#include "NDNodeBase.h"
#include "ndnodeattribute_global.h"
#define g_nodeManager NDNodeManager::getInstance()
class NDNODEATTRIBUTESHARED_EXPORT NDNodeManager : public QObject
{
    Q_OBJECT

public:
    NDNodeManager(QObject* parent = nullptr)  ;
    ~NDNodeManager();

    static NDNodeManager* getInstance(void);

    // 信号通知
    void informAttributeValueChanged(NDAttributeBase* pAttribute, const QVariant& value, bool cmd = false);

signals:
    void signalAttrValueChanged(NDAttributeBase* pAttribute, const QVariant& value, bool cmd = false);
};

#endif
