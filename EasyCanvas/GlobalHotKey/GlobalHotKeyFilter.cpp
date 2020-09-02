#include "GlobalHotKeyFilter.h"
#include "GlobalHotKeyInfo.h"
#include "GlobalHotKeyManager.h"
#include <Windows.h>
#include <qt_windows.h>

GlobalHotKeyFilter::GlobalHotKeyFilter()
{

}

GlobalHotKeyFilter::~GlobalHotKeyFilter()
{

}

bool GlobalHotKeyFilter::nativeEventFilter(const QByteArray & eventType, void * message, long * result)
{
    if (eventType != "windows_generic_MSG")
        return false;

    MSG *param = static_cast<MSG *>(message);

    switch (param->message)
    {
    case WM_HOTKEY:
    {
        m_photKeyManager->disposeHotKey(LOWORD(param->lParam), HIWORD(param->lParam));
        break;
    }
    default:
        break;
    }

    return false;
}
