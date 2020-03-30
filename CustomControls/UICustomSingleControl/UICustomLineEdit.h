#ifndef UICUSTOMLINEEDIT_H
#define UICUSTOMLINEEDIT_H

#include <QLineEdit>
#include <QWidget>
class UICustomLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    UICustomLineEdit(QWidget* parent = nullptr);
    ~UICustomLineEdit();

protected:
    void keyPressEvent(QKeyEvent* event) override;
};

#endif
