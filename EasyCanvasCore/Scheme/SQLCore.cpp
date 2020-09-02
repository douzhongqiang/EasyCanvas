#include "SQLCore.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include "UICanvas/UICanvasItemManager.h"
#include "UICanvas/UICanvasView.h"
#include "NDNodeBase.h"
#include "NDAttributeGroup.h"
#include "NDAttributeBase.h"
#include "UICanvas/UICanvasItemBase.h"

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
    m_schemeTableName = "SchemeManager";
}

// 创建方案表
void SQLCore::createSchemeTables(const QString& schemeName)
{
    // 创建方案表
    QString mainTable("CREATE TABLE IF NOT EXISTS %1 ("
                   "id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,"
                   "nodeName VARCHAR(255) NOT NULL,"
                   "nodeType INT NOT NULL,"
                   "nodeInfo TEXT NOT NULL)");

    mainTable = mainTable.arg(schemeName);
    bool result = m_pSqlQuery->exec(mainTable);
    if (!result)
    {
        qDebug() << m_pSqlQuery->lastError().text();
        return;
    }
}

// 创建方案管理表
void SQLCore::createSchemeManagerTable(void)
{
    QString schemeManagerString("CREATE TABLE IF NOT EXISTS %1 ("
                                "schemeName VARCHAR(200) PRIMARY KEY NOT NULL,"
                                "createTime DATATIME NOT NULL,"
                                "editTime DATATIME NOT NULL,"
                                "indexCount VARCHAR(60) NOT NULL,"
                                "imageWidth INT, "
                                "imageHeight INT,"
                                "image MEDIUMBLOL)");

    schemeManagerString = schemeManagerString.arg(m_schemeTableName);
    bool result = m_pSqlQuery->exec(schemeManagerString);
    if (!result)
        qDebug() << m_pSqlQuery->lastError().text();
}

// 数据存储
void SQLCore::saveData(const QString& schemenName)
{
    // 清除当前的存储
    cleanData(schemenName);

    // 主表
    QString insertString = "INSERT INTO %1 (id, nodeName, nodeType, nodeInfo)"
                           "VALUES(:id, :nodeName, :nodeType, :nodeInfo)";
    insertString = insertString.arg(schemenName);
    m_pSqlQuery->prepare(insertString);

    // 获取全部节点
    QList<NDNodeBase*> nodes;
    NDNodeBase* pSceneNode = g_currentCanvasManager->getCurrentCanvasView()->getCurrentSceneNode();
    nodes << pSceneNode;
    QStringList nodeNames = g_currentCanvasManager->getAllNodeNames();
    foreach (const QString& nodeName, nodeNames)
    {
        NDNodeBase* pNode = g_currentCanvasManager->getNode(nodeName);
        nodes << pNode;
    }

    // 插入数据库
    for (auto iter = nodes.begin(); iter != nodes.end(); ++iter)
    {
        NDNodeBase* pNode = *iter;

        int id = 0;
        QSharedPointer<UICanvasItemBase> canvasItem = g_currentCanvasManager->getCanvasItem(pNode->getNodeName());
        if (!canvasItem.isNull())
            id = canvasItem->getCurrentIndex();

        // Json转为字符串
        QJsonDocument doc(g_currentCanvasManager->getNodeStoreJson(pNode));
        QByteArray byteArray = doc.toJson(QJsonDocument::Compact);
        QString jsonString(byteArray);

        m_pSqlQuery->bindValue(":id", id);
        m_pSqlQuery->bindValue(":nodeName", pNode->getNodeName());
        m_pSqlQuery->bindValue(":nodeType", pNode->getNodeType());
        m_pSqlQuery->bindValue(":nodeInfo", jsonString);

        // 写入主表中
        bool result = m_pSqlQuery->exec();
        if (!result)
        {
            qDebug() << m_pSqlQuery->lastError().text();
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
}

void SQLCore::getSchemeInfoList(std::set<SchemeDataInfo::SchemeInfo>& infos)
{
    infos.clear();
    QSqlQuery query(QString("SELECT schemeName, createTime, editTime, imageWidth, imageHeight, image FROM %1").arg(m_schemeTableName));
    while (query.next())
    {
        SchemeDataInfo::SchemeInfo info;

        info.schemeName = query.value("schemeName").toString();
        info.createTime = query.value("createTime").toDateTime();
        info.editTime = query.value("editTime").toDateTime();

        // 设置图像
        int width = query.value("imageWidth").toInt();
        int height = query.value("imageHeight").toInt();
        if (width > 0 && height > 0)
        {
            // 获取图像数据
            QByteArray byteArray = query.value("image").toByteArray();
            // 数据拷贝
            unsigned char* pImageData = new unsigned char[width * height * 4];
            memcpy(pImageData, byteArray.constData(), byteArray.size());

            QImage image((uchar*)pImageData, width, height, QImage::Format_RGB32);
            info.image = image;
        }

        infos.insert(info);
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
    QString insertString = QString("INSERT INTO %1 (schemeName, createTime, editTime, indexCount, imageWidth, imageHeight, image)"
                           "VALUES(:schemeName, :createTime, :editTime, :indexCount, :imageWidth, :imageHeight, :image)").arg(m_schemeTableName);

    m_pSqlQuery->prepare(insertString);
    m_pSqlQuery->bindValue(":schemeName", info.schemeName);
    m_pSqlQuery->bindValue(":createTime", info.createTime);
    m_pSqlQuery->bindValue(":editTime", info.editTime);
    m_pSqlQuery->bindValue(":imageWidth", info.image.width());
    m_pSqlQuery->bindValue(":imageHeight", info.image.height());


    // 保存图片
    QImage image = info.image;
    if (info.image.format() != QImage::Format_RGB32)
        image = info.image.convertToFormat(QImage::Format_RGB32);
    QByteArray imageArray;
    imageArray.append((const char*)image.constBits(), image.byteCount());
    m_pSqlQuery->bindValue(":image", imageArray);

    // 保存索引列表
    QString indexCountStr = getCurrentIndexCountString();
    m_pSqlQuery->bindValue(":indexCount", indexCountStr);

    bool result = m_pSqlQuery->exec();
    if (!result)
    {
        qDebug() << __FUNCTION__ << m_pSqlQuery->lastError().text();
    }

}

// 编辑方案数据
void SQLCore::editSchemeInfo(const QString& schemeName, const SchemeDataInfo::SchemeInfo& info)
{
    QString updateString("UPDATE %1 SET schemeName=:schemeName, "
                         "createTime=:createTime, "
                         "editTime=:editTime, "
                         "indexCount=:indexCount, "
                         "imageWidth=:imageWidth, "
                         "imageHeight=:imageHeight, "
                         "image=:image "
                         "WHERE schemeName=\'%2\';");
    updateString = updateString.arg(m_schemeTableName).arg(schemeName);
    m_pSqlQuery->prepare(updateString);

    m_pSqlQuery->bindValue(":schemeName", info.schemeName);
    m_pSqlQuery->bindValue(":createTime", info.createTime);
    m_pSqlQuery->bindValue(":editTime", info.editTime);
    m_pSqlQuery->bindValue(":indexCount", getCurrentIndexCountString());
    m_pSqlQuery->bindValue(":imageWidth", info.image.width());
    m_pSqlQuery->bindValue(":imageHeight", info.image.height());

    // 保存图片
    QImage image = info.image;
    if (info.image.format() != QImage::Format_RGB32)
        image = info.image.convertToFormat(QImage::Format_RGB32);
    QByteArray imageArray;
    imageArray.append((const char*)image.constBits(), image.byteCount());
    m_pSqlQuery->bindValue(":image", imageArray);

    bool result = m_pSqlQuery->exec();
    if (!result)
        qDebug() << __FUNCTION__ << m_pSqlQuery->lastError().text();
}

// 删除方案数据
void SQLCore::deleteSchemeInfo(const QString& schemeName)
{
    // 删除管理表中的记录
    QString deleteString("DELETE FROM %1 WHERE schemeName=\'%2\'");
    deleteString = deleteString.arg(m_schemeTableName).arg(schemeName);

    bool result = m_pSqlQuery->exec(deleteString);
    if (!result)
        qDebug() << m_pSqlQuery->lastError().text();

    // 删除属性表
    QString deleteAttrString("DROP TABLE %1;");

    // 删除主表
    QString delString = deleteAttrString.arg(schemeName);
    result = m_pSqlQuery->exec(delString);
    if (!result)
        qDebug() << __FUNCTION__ << m_pSqlQuery->lastError().text();
}

void SQLCore::changedSchemeName(const QString& schemeName, const QString& destName)
{
    // 修该方案表中的名字
    QString updateSqlString = "UPDATE %1 SET schemeName=:schemeName WHERE schemeName = \'%2\'";
    updateSqlString = updateSqlString.arg(m_schemeTableName).arg(schemeName);
    m_pSqlQuery->prepare(updateSqlString);
    m_pSqlQuery->bindValue(":schemeName", destName);
    bool result = m_pSqlQuery->exec();
    if (!result)
        qDebug() << m_pSqlQuery->lastError().text();

    // 修改主表的名字
    QString reNameString = "ALTER TABLE %1 RENAME TO %2";
    reNameString = reNameString.arg(schemeName).arg(destName);
    result = m_pSqlQuery->exec(reNameString);
        if (!result)
            qDebug() << __FUNCTION__ << m_pSqlQuery->lastError().text();
}

void SQLCore::loadScheme(const QString& schemeName)
{
    // 获取数目
    int nTotalCount = 0;
    QString countString = QString("SELECT COUNT(*) FROM %1;").arg(schemeName);
    QSqlQuery query(countString);
    while (query.next()) {
        nTotalCount = query.value(0).toInt();
    }

    // 获取主表中所有节点
    QString schemeMainString = QString("SELECT id, nodeName, nodeType, nodeInfo FROM %1 ORDER BY id;").arg(schemeName);
    QSqlQuery mainQuery(schemeMainString);

    int maxIndex = 0;
    while (mainQuery.next()) {
        // 获取节点名等信息
        QString nodeName = mainQuery.value("nodeName").toString();
        UICanvasItemManager::CanvasItemType nodeType = (UICanvasItemManager::CanvasItemType)mainQuery.value("nodeType").toInt();

        int id = mainQuery.value("id").toInt();
        QString nodeInfo = mainQuery.value("nodeInfo").toString();

        NDNodeBase* pCurrentNode = nullptr;
        // 创建CanvasItem
        if (nodeType != UICanvasItemManager::t_CanvasItem)
        {
            auto canvasItem = g_currentCanvasManager->createCanvasItem(nodeType, nodeName);
            if (canvasItem == nullptr)
                continue;
            canvasItem->setCurrentIndex(id);
            pCurrentNode = canvasItem->getCurrentNode();
        }
        else {
            pCurrentNode = g_currentCanvasManager->getCurrentCanvasView()->getCurrentSceneNode();
        }

        // 设置属性信息
        QJsonParseError jsonParseError;
        QJsonDocument doc = QJsonDocument::fromJson(nodeInfo.toUtf8(), &jsonParseError);
        if (jsonParseError.error != QJsonParseError::NoError)
            continue;
        g_currentCanvasManager->fillNodeJsonInfo(pCurrentNode, doc.object());

        maxIndex = qMax(maxIndex, id);
    }

    g_currentCanvasManager->setCurrentMaxId(++maxIndex);
}
