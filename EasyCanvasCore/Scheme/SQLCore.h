#ifndef SQLCORE_H
#define SQLCORE_H

#include <QObject>
#include <QDateTime>
#include <set>
#include "SchemeDataInfo.h"

class QSqlQuery;
class NDNodeBase;
class SQLCore : public QObject
{
    Q_OBJECT

public:
    explicit SQLCore(QObject* parent = nullptr);
    ~SQLCore();

    // 创建方案表
    void createSchemeTables(const QString& schemeName);
    // 创建方案管理表
    void createSchemeManagerTable(void);

    // 数据存储
    void saveData(const QString& schemenName);
    // 清空存储方案
    void cleanData(const QString& schemenName);

    // 查找
    // 获取方案列表
    void getSchemeInfoList(std::set<SchemeDataInfo::SchemeInfo>& infos);
    // 插入方案数据
    void insertSchemeInfo(const SchemeDataInfo::SchemeInfo& info);
    // 编辑方案数据
    void editSchemeInfo(const QString& schemeName, const SchemeDataInfo::SchemeInfo& info);
    // 删除方案数据
    void deleteSchemeInfo(const QString& schemeName);
    // 更改方案名称
    void changedSchemeName(const QString& schemeName, const QString& destName);

    // 加载方案
    void loadScheme(const QString& schemeName);

private:
    void init(void);

    QSqlQuery *m_pSqlQuery = nullptr;

    QString m_baseAttrAppendString;
    QString m_schemeTableName;

    // 获取当前的索引计数，组合字符串
    QString getCurrentIndexCountString(void);
};

#endif
