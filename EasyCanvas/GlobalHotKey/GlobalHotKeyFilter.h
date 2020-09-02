#ifndef GLOBALHOTKEYFILTER_
#define GLOBALHOTKEYFILTER_

#include <QObject>
#include <QAbstractNativeEventFilter>

class GlobalHotKeyManager;
class GlobalHotKeyFilter : public QAbstractNativeEventFilter
{
public:
    explicit GlobalHotKeyFilter();
    ~GlobalHotKeyFilter();

    friend class GlobalHotKeyManager;
protected:
    bool nativeEventFilter(const QByteArray & eventType, void * message, long * result) override;

private:
    GlobalHotKeyManager* m_photKeyManager = nullptr;
};

#endif
