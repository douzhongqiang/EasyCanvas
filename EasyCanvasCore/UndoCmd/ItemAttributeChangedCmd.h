#ifndef ITEMATTRIBUTECHANGEDCMD_H
#define ITEMATTRIBUTECHANGEDCMD_H

#include <QUndoCommand>
#include <QVariant>
#include <QPointer>
#include <QVector>
#include <QList>

class NDAttributeBase;
class ItemAttributeChangedCmd : public QUndoCommand
{
public:
    ItemAttributeChangedCmd(const QList<NDAttributeBase*>& pAttributeList, const QVariant& value, bool canRun = false);
    ItemAttributeChangedCmd(const QList<NDAttributeBase*>& pAttributeList, const QVector<QVariant>& values, bool canRun = false);
    ~ItemAttributeChangedCmd();

    virtual void undo() override;
    virtual void redo() override;

private:
    QList<QPointer<NDAttributeBase>> m_pAttributeList;
    QVector<QVariant> m_oldValueList;
    QVector<QVariant> m_newValueList;
    bool m_isCanRun = false;
};

#endif
