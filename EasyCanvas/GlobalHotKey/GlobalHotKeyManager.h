#ifndef GLOBALHOTKEYMANAGER_H
#define GLOBALHOTKEYMANAGER_H

#include <QObject>
#include <QSet>
#include <QSharedPointer>
#include "GlobalHotKeyInfo.h"
class GlobalHotKeyManager : public QObject
{
public:
    explicit GlobalHotKeyManager(QObject* parent = nullptr);
    ~GlobalHotKeyManager();

    // 注册快捷键
    void registerHotKey(const QSharedPointer<GlobalHotKeyInfo>& hotKeyInfo);
    // 解除快捷键
    void unregisterHotKey(const QSharedPointer<GlobalHotKeyInfo>& hotKeyInfo, bool needRemove = true);

    bool disposeHotKey(unsigned int modifiers, unsigned int key);

private:
    QSet<QSharedPointer<GlobalHotKeyInfo>> m_sets;

    unsigned int getModifierValue(Qt::KeyboardModifiers value, bool& isOk);
    unsigned int getKeyValue(Qt::Key key, bool& isOk);
};

#endif
