#ifndef UNDOCMDCORE_H
#define UNDOCMDCORE_H

#include <QObject>
#include <QUndoStack>

class UndoCmdCore : public QObject
{
    Q_OBJECT

public:
    enum UndoCmdType
    {
        t_createCmd,
        t_deleteCmd,
        t_renameCmd,
        t_changedAttrCmd
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
    void runChangedAttrCmd(const QString& attrFullName, const QVariant& value);

    void redo(void);
    void undo(void);

private:
    QUndoStack* m_pUndoStack = nullptr;
};

#endif
