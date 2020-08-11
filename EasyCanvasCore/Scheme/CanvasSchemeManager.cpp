#include "CanvasSchemeManager.h"
#include "SQLCore.h"

CanvasSchemeManager* CanvasSchemeManager::getInstance(void)
{
    static CanvasSchemeManager instance;
    return &instance;
}

CanvasSchemeManager::CanvasSchemeManager()
{
    m_pSQLCore = new SQLCore(this);

    // 数据初始化
    init();
}

void CanvasSchemeManager::init(void)
{
    // 创建方案管理表
    m_pSQLCore->createSchemeManagerTable();
    // 同步方案信息
    syncSchemeInfoFromDB();
}

CanvasSchemeManager::~CanvasSchemeManager()
{

}

// 设置/获取当前的方案名称
void CanvasSchemeManager::setCurrentSchemeName(const QString& schemeName)
{
    m_schemeName = schemeName;
}

QString CanvasSchemeManager::getCurrentSchemeName(void)
{
    return m_schemeName;
}

void CanvasSchemeManager::saveScheme(void)
{
    m_pSQLCore->createSchemeTables(m_schemeName);
    m_pSQLCore->saveData(m_schemeName);
}

const QList<SchemeDataInfo::SchemeInfo>& CanvasSchemeManager::getSchemeInfoList(void)
{
    return m_schemeInfos;
}

void CanvasSchemeManager::syncSchemeInfoFromDB(void)
{
    m_pSQLCore->getSchemeInfoList(m_schemeInfos);
}
