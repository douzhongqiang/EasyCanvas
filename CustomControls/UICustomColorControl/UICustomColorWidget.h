#ifndef UICUSTOMCOLORWIDGET_H
#define UICUSTOMCOLORWIDGET_H

#include "UICustomWidgtes/CustomWidget.h"
#include "UICustomColorCircle.h"
#include "UICustomColorSlider.h"
#include "UICustomSingleControl/UICustomColorViewWidget.h"
#include "UICustomCustomColorOperator.h"
#include <QWidget>
#include <QGroupBox>

class QHBoxLayout;
class UICustomColorWidget : public CustomWidget
{
    Q_OBJECT

public:
    UICustomColorWidget(QWidget* parent = nullptr);
    ~UICustomColorWidget();

    // 设置/获取当前的颜色
    void setCurrentColor(const QColor& color);
    QColor getCurrentColor(void);

private:
    UICustomColorCircle* m_pColorCircle = nullptr;

    void initColorCircle(void);

    void initColorSliderWidget(void);
    void initColorRGBSliderWidget(void);
    void initColorHSVSliderWidget(void);
    void initColorButtonWidget(void);
    void initColorCustomWidget(void);

    QGroupBox* m_pRGBGroup = nullptr;
    QGroupBox* m_pHSVGroup = nullptr;
    QGroupBox* m_pCustomGroup = nullptr;

    UICustomColorSlider* m_pRedSlider = nullptr;
    UICustomColorSlider* m_pGreenSlider = nullptr;
    UICustomColorSlider* m_pBlueSlider = nullptr;
    UICustomColorSlider* m_pHSlider = nullptr;
    UICustomColorSlider* m_pSSlider = nullptr;
    UICustomColorSlider* m_pVSlider = nullptr;

    UICustomColorViewWidget* m_pColorView = nullptr;
    QVector<UICustomColorViewWidget*> m_colorViewsList;
    UICustomCustomColorOperator* m_pCustomColorOperator = nullptr;

    QColor m_currentColor;

    // 根据滑条，更新显示
    void updateForColorSlider(void);
    void updateForColorSliderValue(void);

    // 保存和加载
    void saveCustomColor(void);
    void loadCustomColor(void);

private:
    QHBoxLayout* m_pMainLayout = nullptr;
    QVBoxLayout* m_pLayout = nullptr;

private slots:
    void onColorChanged(void);

    void onSliderColorChanged(void);
    void onHSVSliderColorChanged(void);

    // 点击自定义颜色
    void onClickedColorView(void);
    // 点击添加自定义颜色
    void onClickedAddCustomColor(void);
    // 处理自定义颜色信号
    void onDropSignal(void);

signals:
    void clickedOKButton(void);
    void clickedCannelButton(void);
};

#endif
