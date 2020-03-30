#ifndef UICUSTOMDOUBLESPINBOX_H
#define UICUSTOMDOUBLESPINBOX_H

#include <QWidget>
#include <QDoubleSpinBox>

class UICustomDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT

public:
    UICustomDoubleSpinBox(QWidget* parent = nullptr);
    ~UICustomDoubleSpinBox();

protected:
    virtual QSize sizeHint() const override;

    void keyPressEvent(QKeyEvent* event) override;
};

#endif
