#ifndef UICUSTOMTIMEEDIT_H
#define UICUSTOMTIMEEDIT_H

#include <QTimeEdit>
#include <QWidget>

class UICustomTimeEdit : public QTimeEdit
{
    Q_OBJECT

public:
    UICustomTimeEdit(QWidget* parent = nullptr);
    ~UICustomTimeEdit();
};

#endif
