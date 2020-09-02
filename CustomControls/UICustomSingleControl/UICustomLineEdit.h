#ifndef UICUSTOMLINEEDIT_H
#define UICUSTOMLINEEDIT_H

#include <QLineEdit>
#include <QWidget>
#include "UICustomWidgtes/CustomWidget.h"
#include "customcontrols_global.h"

class UICustomCloseButton;
class CUSTOMCONTROLSSHARED_EXPORT UICustomLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    UICustomLineEdit(QWidget* parent = nullptr);
    ~UICustomLineEdit() override;

    // Close Button Visible
    void setCloseButtonEnable(bool isEnabled);

protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    UICustomCloseButton* m_pButton = nullptr;
    bool m_isToolButtonEnable = false;

signals:
    void currentTextChanged(const QString& text);

private slots:
    void onTextChanged(const QString& textChanged);
    void onCloseButtonClicked(void);
};

// ==============================================================
class UICustomCloseButton : public CustomWidget
{
    Q_OBJECT

public:
    UICustomCloseButton(QWidget* parent = nullptr);
    ~UICustomCloseButton();

protected:
    void customPaint(QPainter* painter) override;
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

signals:
    void clicked(void);

private:
    bool m_isHoveredOn = false;
};

#endif
