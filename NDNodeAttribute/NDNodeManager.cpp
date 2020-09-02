#include "NDNodeManager.h"
#include "NDTextNode.h"

NDNodeManager::NDNodeManager(QObject* parent)
    :QObject(parent)
{


}

NDNodeManager::~NDNodeManager()
{

}

NDNodeManager* NDNodeManager::getInstance(void)
{
    static NDNodeManager instance;
    return &instance;
}

void NDNodeManager::informAttributeValueChanged(NDAttributeBase* pAttribute, const QVariant& value, bool cmd)
{
    emit signalAttrValueChanged(pAttribute, value, cmd);
}
