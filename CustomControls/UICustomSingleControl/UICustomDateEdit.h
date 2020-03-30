#ifndef UICUSTOMDATEEDIT_H
#define UICUSTOMDATEEDIT_H

#include <QDateEdit>
class UICutomDateEdit : public QDateEdit
{
    Q_OBJECT

public:
    UICutomDateEdit(QWidget* parent = nullptr);
    ~UICutomDateEdit();
};

#endif
