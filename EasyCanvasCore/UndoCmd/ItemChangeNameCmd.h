#ifndef ITEMCNANGENAMECMD_H
#define ITEMCNANGENAMECMD_H

#include <QUndoCommand>
#include <QSharedPointer>

class UICanvasItemBase;
class ItemChangeNameCmd : public QUndoCommand
{
public:
    explicit ItemChangeNameCmd(QSharedPointer<UICanvasItemBase> pItem, const QString& destName);
    ~ItemChangeNameCmd();

    void undo() override;
    void redo() override;

private:
    QSharedPointer<UICanvasItemBase> m_pCanvasItem;
    QString m_destName;
    QString m_srcName;
};

#endif
