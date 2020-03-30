#ifndef UICUSTOMTEXTEDIT_H
#define UICUSTOMTEXTEDIT_H

#include <QTextEdit>
#include <QWidget>
#include "customcontrols_global.h"

class CUSTOMCONTROLSSHARED_EXPORT UICustomTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    UICustomTextEdit(QWidget* parent = nullptr);
    ~UICustomTextEdit() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    QSize sizeHint(void) const override;

signals:
    void pressedEnterKey(void);         // 按下小回车时，发送此信号
};

#endif
