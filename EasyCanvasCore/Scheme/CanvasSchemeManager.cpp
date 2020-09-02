#include "CanvasSchemeManager.h"
#include "SQLCore.h"
#include "UICanvas/UICanvasItemManager.h"
#include "UICanvas/UICanvasView.h"
#include <QDebug>

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
    // 查找是否存在方案
    SchemeDataInfo::SchemeInfo tempInfo;
    tempInfo.schemeName = m_schemeName;

    auto iter = std::find(m_schemeInfos.begin(), m_schemeInfos.end(), tempInfo);

    if (iter == m_schemeInfos.end())
    {
        // 没找到， 添加方案
        SchemeDataInfo::SchemeInfo info;
        info.schemeName = m_schemeName;
        info.createTime = QDateTime::currentDateTime();
        info.editTime = QDateTime::currentDateTime();

        // 获取图像信息
        QImage image;
        g_currentCanvasManager->getCurrentCanvasView()->saveToImage(image);
        image = converToSamllImage(image);
        info.image = image;

        m_schemeInfos.insert(info);

        // 添加到数据库
        m_pSQLCore->insertSchemeInfo(info);
    }
    else
    {
        // 更新方案
        // 重新设置修改时间
        SchemeDataInfo::SchemeInfo info = *iter;
        info.editTime = QDateTime::currentDateTime();

        // 获取图像信息
        QImage image;
        g_currentCanvasManager->getCurrentCanvasView()->saveToImage(image);
        image = converToSamllImage(image);
        info.image = image;

        m_schemeInfos.erase(iter);
        m_schemeInfos.insert(info);
//        qDebug() << info.editTime.toString("yyyy-MM-dd hh:mm:ss");
//        qDebug() << m_schemeInfos.begin()->editTime.toString("yyyy-MM-dd hh:mm:ss");

        // 更改数据库方案
        m_pSQLCore->editSchemeInfo(info.schemeName, info);
    }

    // 创建方案数据元素表
    m_pSQLCore->createSchemeTables(m_schemeName);
    m_pSQLCore->saveData(m_schemeName);
}

void CanvasSchemeManager::deleteScheme(const QString& schemeName)
{
    SchemeDataInfo::SchemeInfo tempInfo;
    tempInfo.schemeName = schemeName;
    auto iter = m_schemeInfos.find(tempInfo);
    if (iter == m_schemeInfos.end())
        return;

    // 删除方案
    m_pSQLCore->deleteSchemeInfo(schemeName);
    m_schemeInfos.erase(iter);
}

void CanvasSchemeManager::openScheme(void)
{
    m_pSQLCore->loadScheme(m_schemeName);
}

void CanvasSchemeManager::changedSchemeName(const QString& name, const QString& destName)
{
    // 查找
    SchemeDataInfo::SchemeInfo tempInfo;
    tempInfo.schemeName = name;
    auto iter = m_schemeInfos.find(tempInfo);
    if (iter == m_schemeInfos.end())
        return;

    tempInfo = *iter;
    tempInfo.schemeName = destName;
    m_schemeInfos.erase(iter);
    m_schemeInfos.insert(tempInfo);

    m_pSQLCore->changedSchemeName(name, destName);
}

std::set<SchemeDataInfo::SchemeInfo> CanvasSchemeManager::getSchemeInfoList(void)
{
    return m_schemeInfos;
}

// 获取方案名
QString CanvasSchemeManager::getSchemeName(int index)
{
    int count = 0;
    foreach (auto info, m_schemeInfos)
    {
        if (count == index)
            return info.schemeName;

        count++;
    }

    return "";
}

SchemeDataInfo::SchemeInfo CanvasSchemeManager::getSchemeInfo(int index)
{
    int count = 0;
    foreach (auto info, m_schemeInfos)
    {
        if (count == index)
            return info;

        count++;
    }

    return SchemeDataInfo::SchemeInfo();
}

void CanvasSchemeManager::syncSchemeInfoFromDB(void)
{
    m_pSQLCore->getSchemeInfoList(m_schemeInfos);
}

QImage CanvasSchemeManager::converToSamllImage(const QImage& image)
{
    return image.scaled(m_nMaxWidth, m_nMaxHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}
