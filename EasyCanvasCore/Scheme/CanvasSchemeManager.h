#ifndef CANVASSCHEMEMANAGER_H
#define CANVASSCHEMEMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QSet>
#include <set>
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

    // 删除方案
    void deleteScheme(const QString& schemeName);

    // 打开方案
    void openScheme(void);

    // 修改方案名称
    void changedSchemeName(const QString& name, const QString& destName);

    // 获取全部数据
    std::set<SchemeDataInfo::SchemeInfo> getSchemeInfoList(void);
    // 获取方案名
    QString getSchemeName(int index);
    SchemeDataInfo::SchemeInfo getSchemeInfo(int index);

private:
    CanvasSchemeManager();
    ~CanvasSchemeManager();

private:
    // 初始化
    void init(void);

    SQLCore* m_pSQLCore = nullptr;

    QString m_schemeName;

    // 方案管理数据信息
    std::set<SchemeDataInfo::SchemeInfo> m_schemeInfos;
    // 从数据库同步方案
    void syncSchemeInfoFromDB(void);
    // 将图片转化为缩略图
    QImage converToSamllImage(const QImage& image);
    int m_nMaxWidth = 300;
    int m_nMaxHeight = 300;
};

#endif
