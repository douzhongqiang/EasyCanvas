#ifndef UICUSTOMLINEEDITCONTROL_H
#define UICUSTOMLINEEDITCONTROL_H

#include "UICustomCombineControlBase.h"
#include "customcontrols_global.h"
#include "UICustomSingleControl/UICustomLineEdit.h"
#include <QLineEdit>
#include <QToolButton>
#include <QWidget>

class CUSTOMCONTROLSSHARED_EXPORT UICustomLineEditControl : public UICustomCombineControlBase
{
    Q_OBJECT

public:
    UICustomLineEditControl(QWidget* parent = nullptr);
    ~UICustomLineEditControl();

    // 设置按钮相关
    void setToolButtonVisible(bool isVisible);
    void setToolButtonText(const QString& text);
    void setToolButtonIcon(const QIcon& icon);

    // 设置文本
    void setText(const QString& string);
    QString getText(void);

private:
    UICustomLineEdit* m_pLineEdit = nullptr;
    QToolButton* m_pToolButton = nullptr;

signals:
    void clickedToolButton(void);
    void textChanged(const QString &text);
    void editingFinished(void);
};

#endif
