#ifndef GLOBALHOTKEYINFO_H
#define GLOBALHOTKEYINFO_H

#include <QObject>
#include <QWidget>
class GlobalHotKeyManager;
class GlobalHotKeyInfo : public QObject
{
    Q_OBJECT

public:
    static GlobalHotKeyInfo* createInstance(Qt::KeyboardModifiers modifier, Qt::Key key);
    static int m_nTotalKey;

    // 发送激活信号
    void sendActived(void);

    bool operator = (const GlobalHotKeyInfo& info) const{
        return (nKey == info.nKey) || \
               (modifier == info.modifier && key == info.key) || \
               (qtModifier == info.qtModifier && qtKey == info.qtKey);
    }

    friend class GlobalHotKeyManager;

    inline int getId(void) const{
        return nKey;
    }

    ~GlobalHotKeyInfo();
private:
    GlobalHotKeyInfo(QObject* parent = nullptr);

    quint32 key;
    quint32 modifier;

    Qt::KeyboardModifiers qtModifier;
    Qt::Key qtKey;

    int nKey = 0;

signals:
    void actived(void);
};

inline uint qHash(const GlobalHotKeyInfo& info, uint seed = 0)
{
    return qHash(info.getId(), seed);
}

#endif
