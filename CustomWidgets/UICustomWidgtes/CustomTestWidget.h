#ifndef CUSTOMT_TEST_WIDGET_H
#define CUSTOMT_TEST_WIDGET_H

#include <QWidget>
#include "CustomWidget.h"
class QTabWidget;
class QMenuBar;
class CustomTestWidget : public CustomWidget
{
    Q_OBJECT

public:
    CustomTestWidget(QWidget* parent = nullptr);
    ~CustomTestWidget();

private:
    QTabWidget* m_pTabWidget = nullptr;
    QMenuBar* m_pMenuBar = nullptr;

    void initMenuBar(void);
    void initCommonControls(void);
    void initTableControls(void);
    void initListControls(void);
};
#endif
