#ifndef ITEMCREATECMD_H
#define ITEMCREATECMD_H

#include <QUndoCommand>
#include <QSharedPointer>

class UICanvasItemBase;
class ItemCreateCmd : public QUndoCommand
{
public:
    explicit ItemCreateCmd(int type);
    ~ItemCreateCmd();

    void undo() override;
    void redo() override;

private:
    int m_nType = 0;
    QSharedPointer<UICanvasItemBase> m_itemCanvasItem;
};

#endif
