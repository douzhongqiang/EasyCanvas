#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>
#include "customwidgets_global.h"
class CUSTOMWIDGETSSHARED_EXPORT CustomWidget : public QWidget
{
    Q_OBJECT

public:
    CustomWidget(QWidget* parent = nullptr);
    ~CustomWidget();

protected:
    void paintEvent(QPaintEvent* event) final;

    // Custom Paint Event
    virtual void customPaint(QPainter* painter);
};

#endif
