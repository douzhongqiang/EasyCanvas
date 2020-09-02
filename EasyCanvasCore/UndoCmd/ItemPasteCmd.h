#ifndef ITEMPASTECMD_H
#define ITEMPASTECMD_H

#include <QUndoCommand>
#include <QSharedPointer>

class UICanvasItemBase;
class ItemPasteCmd : public QUndoCommand
{
public:
    explicit ItemPasteCmd();
    ~ItemPasteCmd();

    void undo(void) override;
    void redo(void) override;

private:
    QList<QSharedPointer<UICanvasItemBase>> m_createdItems;
};

#endif
