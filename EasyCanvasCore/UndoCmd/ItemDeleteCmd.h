#ifndef ITEMDELETECMD_H
#define ITEMDELETECMD_H

#include <QUndoCommand>
#include <QSharedPointer>

class UICanvasItemBase;
class ItemDeleteCmd : public QUndoCommand
{
public:
    explicit ItemDeleteCmd(QList<QSharedPointer<UICanvasItemBase>> pItems);
    ~ItemDeleteCmd();

    void undo(void) override;
    void redo(void) override;

private:
    QList<QSharedPointer<UICanvasItemBase>> m_pCanvasItems;
};

#endif
