#ifndef UNDOCMDCORE_H
#define UNDOCMDCORE_H

#include <QObject>
#include <QUndoStack>
#include <QUndoView>
#include "easycanvascore_global.h"

class NDAttributeBase;
class EASYCANVASCORESHARED_EXPORT UndoCmdCore : public QObject
{
    Q_OBJECT

public:
    enum UndoCmdType
    {
        t_createCmd,
        t_deleteCmd,
        t_renameCmd,
        t_changedAttrCmd,
        t_pasteCmd
    };

public:
    UndoCmdCore(QObject* parent = nullptr);
    ~UndoCmdCore();

    // 添加 创建节点命令
    void runCreateCmd(int type);
    // 添加 删除节点命令
    void runDeleteCmd(const QStringList& nodeNames);
    // 添加 改变名字命令
    void runChangeNameCmd(const QString& nodeName, const QString& destNodeName);
    // 添加 改变属性命令
    void runChangedAttrCmd(const QString& attrFullName, const QVariant& value, bool isCanrun = false);
    void runChangedAttrCmd(const QList<NDAttributeBase*>& attrList, const QVariant& value, bool isCanrun = false);
    void runChangedAttrCmd(const QList<NDAttributeBase*>& attrList, const QVector<QVariant>& values, bool isCanrun = false);
    // 粘贴命令
    void runPasteCmd(void);

    void redo(void);
    void undo(void);

    // 创建Redo/Undo Action
    QAction* createRedoAction(void);
    QAction* createUndoAction(void);

    // 创建Redo/Undo View
    QUndoView* createUndoView(void);

    // Undo栈相关
    bool isUndoStackEmpty(void);
    void cleanUndoStack(void);

private:
    QUndoStack* m_pUndoStack = nullptr;
};

#endif
