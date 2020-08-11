#include "SQLCore.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include "UICanvas/UICanvasItemManager.h"
#include "NDNodeBase.h"
#include "NDAttributeGroup.h"
#include "NDAttributeBase.h"

SQLCore::SQLCore(QObject* parent)
    :QObject(parent)
{
    init();
}

SQLCore::~SQLCore()
{

}

void SQLCore::init(void)
{
    QSqlDatabase sqlDatabelse = QSqlDatabase::addDatabase("QSQLITE");
    sqlDatabelse.setDatabaseName("./EasyCanvasScheme.db");

    // 打开数据库
    bool result = sqlDatabelse.open();
    if (!result)
        qDebug() << "SQL Open Error!";

    m_pSqlQuery = new QSqlQuery(sqlDatabelse);

    m_baseAttrAppendString = "_BaseAttr";
    m_schemeTableName = "SchemeManager";
}

// 创建方案表
void SQLCore::createSchemeTables(const QString& schemeName)
{
    // 创建方案表
    QString mainTable("CREATE TABLE IF NOT EXISTS %1 ("
                   "nodeName VARCHAR(255) PRIMARY KEY NOT NULL,"
                   "nodeType INT NOT NULL,"
                   "baseAttrIndex INT NOT NULL, "
                   "extendAttrIndex INT NOT NULL)");

    mainTable = mainTable.arg(schemeName);
    bool result = m_pSqlQuery->exec(mainTable);
    if (!result)
    {
        qDebug() << m_pSqlQuery->lastError().text();
        return;
    }

    // 创建方案基础属性表
    QString baseAttrTable("CREATE TABLE IF NOT EXISTS %1 ("
                          "id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,"
                          "xPt DOUBLE NOT NULL,"
                          "yPt DOUBLE NOT NULL, "
                          "zPt DOUBLE NOT NULL,"
                          "width INT NOT NULL,"
                          "height INT NOT NULL,"
                          "rotate DOUBLE NOT NULL)");
    baseAttrTable = baseAttrTable.arg(schemeName + m_baseAttrAppendString);
    result = m_pSqlQuery->exec(baseAttrTable);
    if (!result)
    {
        qDebug() << m_pSqlQuery->lastError().text();
        return;
    }

    // 创建扩展属性表
    for (int i=(int)UICanvasItemManager::t_CanvasItem; i<UICanvasItemManager::t_End; ++i)
    {
        createExternTables(schemeName, i);
    }

}

// 创建方案管理表
void SQLCore::createSchemeManagerTable(void)
{
    QString schemeManagerString("CREATE TABLE IF NOT EXISTS %1 ("
                                "schemeName VARCHAR(200) PRIMARY KEY NOT NULL,"
                                "createTime DATATIME NOT NULL,"
                                "editTime DATATIME NOT NULL, "
                                "indexCount VARCHAR(60) NOT NULL)");

    schemeManagerString = schemeManagerString.arg(m_schemeTableName);
    bool result = m_pSqlQuery->exec(schemeManagerString);
    if (!result)
        qDebug() << m_pSqlQuery->lastError().text();
}

// 创建扩展属性表
void SQLCore::createExternTables(const QString schemeName, int type)
{
    QString createString;
    UICanvasItemManager::CanvasItemType itemType = (UICanvasItemManager::CanvasItemType)type;

    switch (itemType)
    {
    case UICanvasItemManager::t_CanvasItem:
    {
        createString = "CREATE TABLE IF NOT EXISTS %1("
                       "id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,"
                       "width INT NOT NULL,"
                       "height INT NOT NULL,"
                       "canvasColor VARCHAR(20) NOT NULL)";
        break;
    }
    case UICanvasItemManager::t_RectItem:
    {
        createString = "CREATE TABLE IF NOT EXISTS %1("
                       "id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,"
                       "bRounded TINYINT NOT NULL,"
                       "bFillColor TINYINT NOT NULL,"
                       "fillColor VARCHAR(20) NOT NULL, "
                       "bOutline TINYINT NOT NULL,"
                       "outlineWidth INT NOT NULL,"
                       "outlineColor VARCHAR(20) NOT NULL)";
        break;
    }
    case UICanvasItemManager::t_TextItem:
    {
        createString = "CREATE TABLE IF NOT EXISTS %1("
                       "id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,"
                       "text TEXT NOT NULL,"
                       "fontSize INT NOT NULL,"
                       "penSize INT NOT NULL, "
                       "textColor VARCHAR(20) NOT NULL,"
                       "bFillColorOutline TINYINT NOT NULL,"
                       "fillColor VARCHAR(20) NOT NULL,"
                       "bShowOutline TINYINT NOT NULL,"
                       "outlineColor VARCHAR(20) NOT NULL,"
                       "outlineWidth INT NOT NULL)";
        break;
    }
    case UICanvasItemManager::t_EllipseItem:
    {
        createString = "CREATE TABLE IF NOT EXISTS %1("
                       "id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,"
                       "bRounded TINYINT NOT NULL,"
                       "bFillColor TINYINT NOT NULL,"
                       "fillColor VARCHAR(20) NOT NULL, "
                       "bOutline TINYINT NOT NULL,"
                       "outlineWidth INT NOT NULL,"
                       "outlineColor VARCHAR(20) NOT NULL)";
        break;
    }
    case UICanvasItemManager::t_ImageItem:
    {
        createString = "CREATE TABLE IF NOT EXISTS %1("
                       "id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,"
                       "imagePath VARCHAR(255) NOT NULL)";
        break;
    }
    case UICanvasItemManager::t_PathItem:
    {
        createString = "CREATE TABLE IF NOT EXISTS %1("
                       "id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,"
                       "outlineWidth INT NOT NULL,"
                       "outlineColor VARCHAR(20) NOT NULL)";
        break;
    }
    case UICanvasItemManager::t_AudioItem:
    {
        createString = "CREATE TABLE IF NOT EXISTS %1("
                       "id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,"
                       "path VARCHAR(255) NOT NULL,"
                       "startColor VARCHAR(20) NOT NULL,"
                       "endColor VARCHAR(20) NOT NULL)";
        break;
    }
    default:
        return;
    }

    createString = createString.arg(schemeName + "_" + g_currentCanvasManager->getNodeTypeDisplayName(itemType));
    bool result = m_pSqlQuery->exec(createString);
    if (!result)
        qDebug() << m_pSqlQuery->lastError().text();
}

QString SQLCore::getExternAttrInsertString(const QString& schemeName, int type)
{
    QString insertString;
    UICanvasItemManager::CanvasItemType itemType = (UICanvasItemManager::CanvasItemType)type;

    switch (itemType)
    {
    case UICanvasItemManager::t_CanvasItem:
    {
        insertString = "INSERT INTO %1 (id, width, height, canvasColor)"
                       "VALUES(:id, :width, :height, :canvasColor)";

        break;
    }
    case UICanvasItemManager::t_RectItem:
    {
        insertString = "INSERT INTO %1 (id, bRounded, bFillColor, fillColor, bOutline, outlineWidth, outlineColor)"
                       "VALUES(:id, :bRounded, :bFillColor, :fillColor, :bOutline, :outlineWidth, :outlineColor)";

        break;
    }
    case UICanvasItemManager::t_TextItem:
    {
        insertString = "INSERT INTO %1 (id, text, fontSize, penSize, textColor, bFillColorOutline, fillColor, bShowOutline, outlineColor, outlineWidth)"
                       "VALUES(:id, :text, :fontSize, :penSize, :textColor, :bFillColorOutline, :fillColor, :bShowOutline, :outlineColor, :outlineWidth)";
        break;
    }
    case UICanvasItemManager::t_EllipseItem:
    {
        insertString = "INSERT INTO %1 (id, bRounded, bFillColor, fillColor, bOutline, outlineWidth, outlineColor)"
                       "VALUES(:id, :bRounded, :bFillColor, :fillColor, :bOutline, :outlineWidth, :outlineColor)";
        break;
    }
    case UICanvasItemManager::t_ImageItem:
    {
        insertString = "INSERT INTO %1 (id, imagePath)"
                       "VALUES(:id, :imagePath)";
        break;
    }
    case UICanvasItemManager::t_PathItem:
    {
        insertString = "INSERT INTO %1 (id, outlineWidth, outlineColor)"
                       "VALUES(:id, :outlineWidth, :outlineColor)";
        break;
    }
    case UICanvasItemManager::t_AudioItem:
    {
        insertString = "INSERT INTO %1 (id, path, startColor, endColor)"
                       "VALUES(:id, :path, :startColor, :endColor)";
        break;
    }
    default:
        return "";
    }

    return insertString.arg(schemeName + "_" + g_currentCanvasManager->getNodeTypeDisplayName(itemType));
}

// 数据存储
void SQLCore::saveData(const QString& schemenName)
{
    // 清除当前的存储
    cleanData(schemenName);

    // 基础属性
    QString insertBaseAttrString = "INSERT INTO %1 (id, xPt, yPt, zPt, width, height, rotate)"
                                   "VALUES(:id, :xPt, :yPt, :zPt, :width, :height, :rotate)";
    insertBaseAttrString = insertBaseAttrString.arg(schemenName + m_baseAttrAppendString);


    // 主表
    QString insertString = "INSERT INTO %1 (nodeName, nodeType, baseAttrIndex, extendAttrIndex)"
                           "VALUES(:nodeName, :nodeType, :baseAttrIndex, :extendAttrIndex)";
    insertString = insertString.arg(schemenName);

    // 插入数据库
    QStringList nodeNames = g_currentCanvasManager->getAllNodeNames();
    foreach (const QString& nodeName, nodeNames)
    {
        NDNodeBase* pNode = g_currentCanvasManager->getNode(nodeName);

        // 获取属性组
        QList<NDAttributeGroup*> attrGroups;
        pNode->getAllAttributeGroups(attrGroups);

        int nBaseAttributeIndex = 0;
        int nExternAttributeIndex = 0;

        // 写入到数据库
        int count = 0;
        for (auto iter = attrGroups.begin(); iter != attrGroups.end(); ++iter)
        {
            count++;
            // 获取属性列表
            QList<NDAttributeBase*> attrs;
            (*iter)->getAttributes(attrs);

            bool isBaseAttr = false;
            if ((*iter)->getName() == "BaseAttr")
            {
                isBaseAttr = true;

                // 获取最大数目
                QString getMaxNumberString("SELECT MAX(id) FROM %1");
                getMaxNumberString = getMaxNumberString.arg(schemenName + m_baseAttrAppendString);
                QSqlQuery query(getMaxNumberString);
                while (query.next())
                    nBaseAttributeIndex = query.value(0).toInt();

                // 设置基础属性表
                m_pSqlQuery->prepare(insertBaseAttrString);
            }
            else
            {
                if (count <= 2)
                {
                    // 获取最大数目
                    QString getMaxNumberString("SELECT MAX(id) FROM %1");
                    UICanvasItemManager::CanvasItemType canvasType = (UICanvasItemManager::CanvasItemType)pNode->getNodeType();
                    getMaxNumberString = getMaxNumberString.arg(schemenName + "_" + g_currentCanvasManager->getNodeTypeDisplayName(canvasType));
                    QSqlQuery query(getMaxNumberString);
                    while (query.next())
                        nExternAttributeIndex = query.value(0).toInt();

                    // 设置扩展属性表
                    QString str = getExternAttrInsertString(schemenName, pNode->getNodeType());
                    m_pSqlQuery->prepare(str);
                }
            }

            // 写入属性
            for (auto iter = attrs.begin(); iter != attrs.end(); ++iter)
            {
                QString attrName = QString(":") + (*iter)->getName();
                QVariant attrValue = NDAttributeBase::getCurrentValue(*iter);

                // 对于颜色，转成字符串
                if ((*iter)->Type() == NDAttributeBase::t_color)
                {
                    QColor color = attrValue.value<QColor>();

                    QString colorStr("%1,%2,%3");
                    colorStr = colorStr.arg(color.red()).arg(color.green()).arg(color.blue());
                    attrValue.setValue(colorStr);
                }

                m_pSqlQuery->bindValue(attrName, attrValue);
            }

            // 写入基础属性表中
            if (isBaseAttr)
            {
                m_pSqlQuery->bindValue(":id", ++nBaseAttributeIndex);
                bool result = m_pSqlQuery->exec();
                if (!result)
                {
                    qDebug() << m_pSqlQuery->lastError().text();
                    continue;
                }
            }
        }

        m_pSqlQuery->bindValue(":id", ++nExternAttributeIndex);
        bool result = m_pSqlQuery->exec();
        if (!result)
        {
            qDebug() << m_pSqlQuery->lastError().text();
            continue;
        }

        // 写入主表中
        m_pSqlQuery->prepare(insertString);
        m_pSqlQuery->bindValue(":nodeName", pNode->getNodeName());
        m_pSqlQuery->bindValue(":nodeType", pNode->getNodeType());
        m_pSqlQuery->bindValue(":baseAttrIndex", nBaseAttributeIndex);
        m_pSqlQuery->bindValue(":extendAttrIndex", nExternAttributeIndex);
        result = m_pSqlQuery->exec();
        if (!result)
        {
            qDebug() << m_pSqlQuery->lastError().text();
            continue;
        }
    }
}

void SQLCore::cleanData(const QString& schemenName)
{
    QString deleteString("DELETE FROM %1;");

    // 删除主表
    QString delString = deleteString.arg(schemenName);
    bool result = m_pSqlQuery->exec(delString);
    if (!result)
        qDebug() << m_pSqlQuery->lastError().text();

    // 删除基础属性表
    QString delString2 = deleteString.arg(schemenName + m_baseAttrAppendString);
    result = m_pSqlQuery->exec(delString2);
    if (!result)
        qDebug() << m_pSqlQuery->lastError().text();

    // 删除扩展属性表
    for (int i=(int)UICanvasItemManager::t_RectItem; i<UICanvasItemManager::t_End; ++i)
    {
        UICanvasItemManager::CanvasItemType itemType = (UICanvasItemManager::CanvasItemType)i;
        QString tableName = schemenName + "_" + g_currentCanvasManager->getNodeTypeDisplayName(itemType);

        QString delString3 = deleteString.arg(tableName);
        result = m_pSqlQuery->exec(delString3);
        if (!result)
            qDebug() << m_pSqlQuery->lastError().text();
    }
}

void SQLCore::getSchemeInfoList(QList<SchemeDataInfo::SchemeInfo>& infos)
{
    infos.clear();
    QSqlQuery query(QString("SELECT schemeName, createTime, editTime FROM %1").arg(m_schemeTableName));
    while (query.next())
    {
        SchemeDataInfo::SchemeInfo info;

        info.schemeName = query.value("schemeName").toString();
        info.createTime = query.value("createTime").toDateTime();
        info.editTime = query.value("editTime").toDateTime();

        infos << info;
    }
}

QString SQLCore::getCurrentIndexCountString(void)
{
    QStringList strList;
    for (int i=(int)UICanvasItemManager::t_RectItem; i<(int)UICanvasItemManager::t_End; ++i)
    {
        UICanvasItemManager::CanvasItemType itemType = (UICanvasItemManager::CanvasItemType)i;
        int indexCount = g_currentCanvasManager->getNodeCounts(itemType);
        strList << QString::number(indexCount);
    }

    return strList.join(",");
}

// 插入方案数据
void SQLCore::insertSchemeInfo(const SchemeDataInfo::SchemeInfo& info)
{
    QString insertString = QString("INSERT INTO %1 (schemeName, createTime, editTime, indexCount)"
                           "VALUES(:schemeName, :createTime, :editTime, :indexCount)").arg(m_schemeTableName);

    m_pSqlQuery->prepare(insertString);
    m_pSqlQuery->bindValue(":schemeName", info.schemeName);
    m_pSqlQuery->bindValue(":createTime", info.createTime);
    m_pSqlQuery->bindValue(":editTime", info.editTime);

    QString indexCountStr = getCurrentIndexCountString();
    m_pSqlQuery->bindValue(":indexCount", indexCountStr);

    bool result = m_pSqlQuery->exec();
    if (!result)
        qDebug() << m_pSqlQuery->lastError().text();

}

// 编辑方案数据
void SQLCore::editSchemeInfo(const QString& schemeName, const SchemeDataInfo::SchemeInfo& info)
{
    QString updateString("UPDATE %1 SET schemeName=\'%2\', createTime=%3, editTime=%4, indexCount=%5 WHERE schemeName=\'%6\';");

    //updateString = updateString.arg();
}

// 删除方案数据
void SQLCore::deleteSchemeInfo(const QString& schemeName)
{

}

void SQLCore::loadScheme(const QString& schemeName)
{

}
