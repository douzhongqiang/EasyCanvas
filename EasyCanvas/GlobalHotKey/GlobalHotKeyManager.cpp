#include "GlobalHotKeyManager.h"
#include "GlobalHotKeyFilter.h"
#include <QDebug>
#include <Windows.h>
#include <QApplication>
#pragma comment(lib, "user32.lib")

GlobalHotKeyManager::GlobalHotKeyManager(QObject* parent)
    :QObject(parent)
{
    GlobalHotKeyFilter* filer = new GlobalHotKeyFilter;
    filer->m_photKeyManager = this;
    qApp->installNativeEventFilter(filer);
}

GlobalHotKeyManager::~GlobalHotKeyManager()
{
    for (auto iter = m_sets.begin(); iter != m_sets.end(); ++iter)
        unregisterHotKey(*iter, false);
    m_sets.clear();
}

// 注册快捷键
void GlobalHotKeyManager::registerHotKey(const QSharedPointer<GlobalHotKeyInfo>& hotKeyInfo)
{
    bool result = false;
    unsigned int modifier = getModifierValue(hotKeyInfo->qtModifier, result);
    if (!result)
        return;

    unsigned int key = getKeyValue(hotKeyInfo->qtKey, result);
    if (!result)
        return;

    hotKeyInfo->modifier = modifier;
    hotKeyInfo->key = key;
    m_sets.insert(hotKeyInfo);
    result = ::RegisterHotKey(nullptr, hotKeyInfo->nKey, modifier, key);
    if (!result)
        qDebug() << "Fail Register Hot Key";
}

// 解除快捷键
void GlobalHotKeyManager::unregisterHotKey(const QSharedPointer<GlobalHotKeyInfo>& hotKeyInfo, bool needRemove)
{
    if (m_sets.find(hotKeyInfo) == m_sets.end())
        return;

    ::UnregisterHotKey(nullptr, hotKeyInfo->nKey);

    if (needRemove)
        m_sets.remove(hotKeyInfo);
}

bool GlobalHotKeyManager::disposeHotKey(unsigned int modifiers, unsigned int key)
{
    QSharedPointer<GlobalHotKeyInfo> pInfo(new GlobalHotKeyInfo);
    pInfo->modifier = modifiers;
    pInfo->key = key;

    for (auto iter = m_sets.begin(); iter != m_sets.end(); ++iter)
    {
        if ((*iter)->key == key && (*iter)->modifier == modifiers)
        {
            (*iter)->sendActived();
            return true;

        }
    }

    return false;
}

unsigned int GlobalHotKeyManager::getModifierValue(Qt::KeyboardModifiers value, bool& isOk)
{
    unsigned int modife = 0;
    if (value & Qt::ControlModifier)
        modife |= MOD_CONTROL;
    if (value & Qt::AltModifier)
        modife |= MOD_ALT;
    if (value & Qt::ShiftModifier)
        modife |= MOD_SHIFT;

    if (modife)
        isOk = true;
    else {
        isOk = false;
    }

    return modife;
}

unsigned int GlobalHotKeyManager::getKeyValue(Qt::Key keycode, bool& isOk)
{
    isOk = true;
    if(keycode <= 0xFFFF) {//Try to obtain the key from it's "character"
        const SHORT vKey = VkKeyScanW(keycode);
        if(vKey > -1)
            return LOBYTE(vKey);
    }

    //find key from switch/case --> Only finds a very small subset of keys
    switch (keycode)
    {
    case Qt::Key_Escape:
        return VK_ESCAPE;
    case Qt::Key_Tab:
    case Qt::Key_Backtab:
        return VK_TAB;
    case Qt::Key_Backspace:
        return VK_BACK;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        return VK_RETURN;
    case Qt::Key_Insert:
        return VK_INSERT;
    case Qt::Key_Delete:
        return VK_DELETE;
    case Qt::Key_Pause:
        return VK_PAUSE;
    case Qt::Key_Print:
        return VK_PRINT;
    case Qt::Key_Clear:
        return VK_CLEAR;
    case Qt::Key_Home:
        return VK_HOME;
    case Qt::Key_End:
        return VK_END;
    case Qt::Key_Left:
        return VK_LEFT;
    case Qt::Key_Up:
        return VK_UP;
    case Qt::Key_Right:
        return VK_RIGHT;
    case Qt::Key_Down:
        return VK_DOWN;
    case Qt::Key_PageUp:
        return VK_PRIOR;
    case Qt::Key_PageDown:
        return VK_NEXT;
    case Qt::Key_CapsLock:
        return VK_CAPITAL;
    case Qt::Key_NumLock:
        return VK_NUMLOCK;
    case Qt::Key_ScrollLock:
        return VK_SCROLL;

    case Qt::Key_F1:
        return VK_F1;
    case Qt::Key_F2:
        return VK_F2;
    case Qt::Key_F3:
        return VK_F3;
    case Qt::Key_F4:
        return VK_F4;
    case Qt::Key_F5:
        return VK_F5;
    case Qt::Key_F6:
        return VK_F6;
    case Qt::Key_F7:
        return VK_F7;
    case Qt::Key_F8:
        return VK_F8;
    case Qt::Key_F9:
        return VK_F9;
    case Qt::Key_F10:
        return VK_F10;
    case Qt::Key_F11:
        return VK_F11;
    case Qt::Key_F12:
        return VK_F12;
    case Qt::Key_F13:
        return VK_F13;
    case Qt::Key_F14:
        return VK_F14;
    case Qt::Key_F15:
        return VK_F15;
    case Qt::Key_F16:
        return VK_F16;
    case Qt::Key_F17:
        return VK_F17;
    case Qt::Key_F18:
        return VK_F18;
    case Qt::Key_F19:
        return VK_F19;
    case Qt::Key_F20:
        return VK_F20;
    case Qt::Key_F21:
        return VK_F21;
    case Qt::Key_F22:
        return VK_F22;
    case Qt::Key_F23:
        return VK_F23;
    case Qt::Key_F24:
        return VK_F24;

    case Qt::Key_Menu:
        return VK_APPS;
    case Qt::Key_Help:
        return VK_HELP;
    case Qt::Key_MediaNext:
        return VK_MEDIA_NEXT_TRACK;
    case Qt::Key_MediaPrevious:
        return VK_MEDIA_PREV_TRACK;
    case Qt::Key_MediaPlay:
        return VK_MEDIA_PLAY_PAUSE;
    case Qt::Key_MediaStop:
        return VK_MEDIA_STOP;
    case Qt::Key_VolumeDown:
        return VK_VOLUME_DOWN;
    case Qt::Key_VolumeUp:
        return VK_VOLUME_UP;
    case Qt::Key_VolumeMute:
        return VK_VOLUME_MUTE;
    case Qt::Key_Mode_switch:
        return VK_MODECHANGE;
    case Qt::Key_Select:
        return VK_SELECT;
    case Qt::Key_Printer:
        return VK_PRINT;
    case Qt::Key_Execute:
        return VK_EXECUTE;
    case Qt::Key_Sleep:
        return VK_SLEEP;
    case Qt::Key_Period:
        return VK_DECIMAL;
    case Qt::Key_Play:
        return VK_PLAY;
    case Qt::Key_Cancel:
        return VK_CANCEL;

    case Qt::Key_Forward:
        return VK_BROWSER_FORWARD;
    case Qt::Key_Refresh:
        return VK_BROWSER_REFRESH;
    case Qt::Key_Stop:
        return VK_BROWSER_STOP;
    case Qt::Key_Search:
        return VK_BROWSER_SEARCH;
    case Qt::Key_Favorites:
        return VK_BROWSER_FAVORITES;
    case Qt::Key_HomePage:
        return VK_BROWSER_HOME;

    case Qt::Key_LaunchMail:
        return VK_LAUNCH_MAIL;
    case Qt::Key_LaunchMedia:
        return VK_LAUNCH_MEDIA_SELECT;
    case Qt::Key_Launch0:
        return VK_LAUNCH_APP1;
    case Qt::Key_Launch1:
        return VK_LAUNCH_APP2;

    case Qt::Key_Massyo:
        return VK_OEM_FJ_MASSHOU;
    case Qt::Key_Touroku:
        return VK_OEM_FJ_TOUROKU;

    default:
        if(keycode <= 0xFFFF)
            return (byte)keycode;
        else {
            isOk = false;
            return 0;
        }
    }
}
