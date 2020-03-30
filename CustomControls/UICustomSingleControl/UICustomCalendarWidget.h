#ifndef UICUSTOMCALENDARWIDGET_H
#define UICUSTOMCALENDARWIDGET_H

#include <QCalendarWidget>

class UICustomCalendarWidget : public QCalendarWidget
{
    Q_OBJECT

public:
    UICustomCalendarWidget(QWidget* parent = nullptr);
    ~UICustomCalendarWidget();
};

#endif
