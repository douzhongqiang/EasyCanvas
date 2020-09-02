#include "GlobalHotKeyInfo.h"

int GlobalHotKeyInfo::m_nTotalKey = 0;

GlobalHotKeyInfo* GlobalHotKeyInfo::createInstance(Qt::KeyboardModifiers modifier, Qt::Key key)
{
    GlobalHotKeyInfo* pInfo = new GlobalHotKeyInfo;
    pInfo->nKey = ++m_nTotalKey;
    pInfo->qtKey = key;
    pInfo->qtModifier = modifier;

    return pInfo;
}

GlobalHotKeyInfo::GlobalHotKeyInfo(QObject* parent)
    :QObject(parent)
{

}

GlobalHotKeyInfo::~GlobalHotKeyInfo()
{

}

void GlobalHotKeyInfo::sendActived(void)
{
    emit actived();
}
