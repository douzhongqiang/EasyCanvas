#ifndef CANVASSCHEMEMANAGER_H
#define CANVASSCHEMEMANAGER_H

#include <QObject>
#include <QDateTime>
#include "easycanvascore_global.h"
#include "SchemeDataInfo.h"

#define g_CanvasSchemeManager CanvasSchemeManager::getInstance()

class SQLCore;
class EASYCANVASCORESHARED_EXPORT CanvasSchemeManager : public QObject
{
    Q_OBJECT

public:
    static CanvasSchemeManager* getInstance(void);

    // 设置/获取当前的方案名称
    void setCurrentSchemeName(const QString& schemeName);
    QString getCurrentSchemeName(void);

    // 保存当前方案
    void saveScheme(void);

    const QList<SchemeDataInfo::SchemeInfo>& getSchemeInfoList(void);

private:
    CanvasSchemeManager();
    ~CanvasSchemeManager();

private:
    // 初始化
    void init(void);

    SQLCore* m_pSQLCore = nullptr;

    QString m_schemeName;

    // 方案管理数据信息
    QList<SchemeDataInfo::SchemeInfo> m_schemeInfos;
    // 从数据库同步方案
    void syncSchemeInfoFromDB(void);
};

#endif
