#ifndef UICUSTOMPOSTIONCONTROL_H
#define UICUSTOMPOSTIONCONTROL_H

#include "UICustomCombineControlBase.h"
#include "UICustomSingleControl/UICustomPostionWidget.h"
#include "customcontrols_global.h"
#include <QWidget>

class CUSTOMCONTROLSSHARED_EXPORT UICustomPostionControl : public UICustomCombineControlBase
{
    Q_OBJECT

public:
    UICustomPostionControl(QWidget* parent = nullptr);
    ~UICustomPostionControl();

    // 设置当前映射的尺寸
    void setCurrentMapSize(int width, int height);

    // 设置当前的位置
    void setCurrentXPt(int xValue);
    void setCurrentYPt(int yValue);
    void setCurrentPostion(int xValue, int yValue);

    // 获取当前位置
    int getCurrentXPt(void);
    int getCurrentYPt(void);
    void getCurrentPostion(int& xValue, int& yValue);

private:
    UICustomPostionWidget* m_pPostionWidget = nullptr;

    int m_nMapWidth = 100;
    int m_nMapHeight = 100;

signals:
    void postionValueChanged(qreal xValue, qreal yValue);
};

#endif
