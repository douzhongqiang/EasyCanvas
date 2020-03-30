#ifndef RALLBLOCKSIGNAL_H
#define RALLBLOCKSIGNAL_H

#include <QObject>
#include "utils_global.h"

class UTILSSHARED_EXPORT RALLBlockSingal
{
public:
    RALLBlockSingal(QObject* object);
    virtual ~RALLBlockSingal();

private:
    QObject* m_pObject = nullptr;
};

#endif
