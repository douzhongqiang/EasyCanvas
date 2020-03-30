#include "RALLBlockSignal.h"

RALLBlockSingal::RALLBlockSingal(QObject* object)
    :m_pObject(object)
{
    object->blockSignals(true);
}

RALLBlockSingal::~RALLBlockSingal()
{
    m_pObject->blockSignals(false);
}
