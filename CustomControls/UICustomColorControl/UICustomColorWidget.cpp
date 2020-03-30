#include "UICustomColorWidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>

UICustomColorWidget::UICustomColorWidget(QWidget* parent)
    :CustomWidget(parent)
    ,m_currentColor(0, 0, 0)
{
    // CustomColor
    m_pCustomColorOperator = new UICustomCustomColorOperator(this);

    m_pLayout = new QVBoxLayout(this);
    m_pLayout->setSpacing(0);

    QWidget* pW = new QWidget;
    pW->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pLayout->addWidget(pW);

    m_pColorCircle = new UICustomColorCircle;
    m_pColorCircle->setFixedSize(300, 300);

    m_pMainLayout = new QHBoxLayout(pW);
    m_pMainLayout->setMargin(0);
    initColorCircle();
    initColorSliderWidget();
    initColorButtonWidget();

    QObject::connect(m_pColorCircle, &UICustomColorCircle::colorChanged, \
                     this, &UICustomColorWidget::onColorChanged);
}

UICustomColorWidget::~UICustomColorWidget()
{

}

void UICustomColorWidget::initColorCircle(void)
{
    QWidget* widget = new QWidget;
    m_pMainLayout->addWidget(widget);

    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setMargin(2);
    layout->addWidget(m_pColorCircle);
}

void UICustomColorWidget::initColorSliderWidget(void)
{
    QWidget* widget = new QWidget;
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pMainLayout->addWidget(widget);

    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setMargin(0);
    layout->setSpacing(2);

    initColorRGBSliderWidget();
    initColorHSVSliderWidget();

    layout->addWidget(m_pRGBGroup);
    layout->addWidget(m_pHSVGroup);

    layout->addStretch();
}

void UICustomColorWidget::initColorHSVSliderWidget(void)
{
    m_pHSVGroup = new QGroupBox("HSV");

    QVBoxLayout *playout = new QVBoxLayout(m_pHSVGroup);
    m_pHSlider = new UICustomColorSlider;
    m_pHSlider->setTagText("H: ");
    m_pHSlider->setHVisible(true);
    m_pHSlider->setValueRange(0, 359);
    playout->addWidget(m_pHSlider);
    QObject::connect(m_pHSlider, &UICustomColorSlider::valueChanged, \
                     this, &UICustomColorWidget::onHSVSliderColorChanged);

    m_pSSlider = new UICustomColorSlider;
    m_pSSlider->setTagText("S: ");
    m_pSSlider->setValueRange(0, 255);
    playout->addWidget(m_pSSlider);
    QObject::connect(m_pSSlider, &UICustomColorSlider::valueChanged, \
                     this, &UICustomColorWidget::onHSVSliderColorChanged);

    m_pVSlider = new UICustomColorSlider;
    m_pVSlider->setTagText("V: ");
    m_pVSlider->setValueRange(0, 255);
    playout->addWidget(m_pVSlider);
    QObject::connect(m_pVSlider, &UICustomColorSlider::valueChanged, \
                     this, &UICustomColorWidget::onHSVSliderColorChanged);
}

void UICustomColorWidget::initColorButtonWidget(void)
{
    QWidget* bottomWidget = new QWidget;
    bottomWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout* mainLayout = new QHBoxLayout(bottomWidget);
    mainLayout->setMargin(0);

    // 添加自定义颜色
    initColorCustomWidget();
    mainLayout->addWidget(m_pCustomGroup);

    m_pColorView = new UICustomColorViewWidget;
    m_pColorView->setDragVisible(true);
    m_pColorView->setFixedSize(120, 120);
    mainLayout->addWidget(m_pColorView);

    QVBoxLayout *bottomLayout = new QVBoxLayout;
    bottomLayout->addStretch();

    // 添加到自定义按钮
    QPushButton* addButton = new QPushButton(tr("AddToCustom"));
    addButton->setFixedSize(120, 30);
    bottomLayout->addWidget(addButton);
    QObject::connect(addButton, &QPushButton::clicked, this, &UICustomColorWidget::onClickedAddCustomColor);

    // 确定按钮
    QPushButton* okButton = new QPushButton(tr("OK"));
    okButton->setFixedSize(120, 30);
    bottomLayout->addWidget(okButton);
    QObject::connect(okButton, &QPushButton::clicked, this, &UICustomColorWidget::clickedOKButton);

    // 取消按钮
    QPushButton* cannelButton = new QPushButton(tr("Cannel"));
    cannelButton->setFixedSize(120, 30);
    bottomLayout->addWidget(cannelButton);
    mainLayout->addLayout(bottomLayout);
    QObject::connect(cannelButton, &QPushButton::clicked, this, &UICustomColorWidget::clickedCannelButton);

    m_pLayout->addWidget(bottomWidget);
}

void UICustomColorWidget::initColorCustomWidget(void)
{
    m_pCustomGroup = new QGroupBox(tr("Custom Color"));
    QGridLayout *layout = new QGridLayout(m_pCustomGroup);
    layout->setMargin(0);
    layout->setSpacing(0);

    for (int i=0; i<14; ++i)
    {
        UICustomColorViewWidget* colorView = new UICustomColorViewWidget;
        colorView->setAcceptDrops(true);
        colorView->setFixedSize(70, 30);

        layout->addWidget(colorView, i / (14 / 2), i % (14 / 2));
        m_colorViewsList << colorView;

        // 点击时，设置颜色
        QObject::connect(colorView, &UICustomColorViewWidget::mousePressed, \
                         this, &UICustomColorWidget::onClickedColorView);
        QObject::connect(colorView, &UICustomColorViewWidget::dropSignal, \
                         this, &UICustomColorWidget::onDropSignal);
    }

    // 保存到文件
    loadCustomColor();
}

// 点击自定义颜色
void UICustomColorWidget::onClickedColorView(void)
{
    UICustomColorViewWidget* pColorView = dynamic_cast<UICustomColorViewWidget*>(sender());
    if (pColorView == nullptr)
        return;

    // 清除选择
    for (auto iter = m_colorViewsList.begin(); iter != m_colorViewsList.end(); ++iter)
        (*iter)->setSelected(false);

    // 设置选中
    pColorView->setSelected(true);

    // 设置当前的颜色
    QColor color = pColorView->getCurrentColor();
    setCurrentColor(color);
}

void UICustomColorWidget::onClickedAddCustomColor(void)
{
    // 设置选中的为当前颜色
    for (auto iter = m_colorViewsList.begin(); iter != m_colorViewsList.end(); ++iter)
    {
        bool isSelected = (*iter)->getSelected();
        if (isSelected)
        {
            (*iter)->setCurrentColor(m_currentColor);
            break;
        }
    }

    // 保存到文件
    saveCustomColor();
}

void UICustomColorWidget::onDropSignal(void)
{
    // 保存到文件
    saveCustomColor();
}

void UICustomColorWidget::initColorRGBSliderWidget(void)
{
    m_pRGBGroup = new QGroupBox("RGB");
    QVBoxLayout *playout = new QVBoxLayout(m_pRGBGroup);

    m_pRedSlider = new UICustomColorSlider;
    m_pRedSlider->setTagText("R: ");
    playout->addWidget(m_pRedSlider);
    m_pRedSlider->setValueRange(0, 255);
    m_pRedSlider->setStartColor(QColor(0, m_currentColor.green(), m_currentColor.blue()));
    m_pRedSlider->setEndColor(QColor(255, m_currentColor.green(), m_currentColor.blue()));
    QObject::connect(m_pRedSlider, &UICustomColorSlider::valueChanged, \
                     this, &UICustomColorWidget::onSliderColorChanged);

    m_pGreenSlider = new UICustomColorSlider;
    m_pGreenSlider->setTagText("G: ");
    playout->addWidget(m_pGreenSlider);
    m_pGreenSlider->setValueRange(0, 255);
    m_pGreenSlider->setStartColor(QColor(m_currentColor.red(), 0, m_currentColor.blue()));
    m_pGreenSlider->setEndColor(QColor(m_currentColor.red(), 255, m_currentColor.blue()));
    QObject::connect(m_pGreenSlider, &UICustomColorSlider::valueChanged, \
                     this, &UICustomColorWidget::onSliderColorChanged);

    m_pBlueSlider = new UICustomColorSlider;
    m_pBlueSlider->setTagText("B: ");
    playout->addWidget(m_pBlueSlider);
    m_pBlueSlider->setValueRange(0, 255);
    m_pBlueSlider->setStartColor(QColor(m_currentColor.red(), m_currentColor.green(), 0));
    m_pBlueSlider->setEndColor(QColor(m_currentColor.red(), m_currentColor.green(), 255));
    QObject::connect(m_pBlueSlider, &UICustomColorSlider::valueChanged, \
                     this, &UICustomColorWidget::onSliderColorChanged);
}

void UICustomColorWidget::onColorChanged(void)
{
    QColor color = m_pColorCircle->getCurrentColor();
    m_pColorView->setCurrentColor(color);

    m_currentColor = color;

    // 更新Color颜色显示
    updateForColorSlider();
    updateForColorSliderValue();
}

void UICustomColorWidget::onSliderColorChanged(void)
{
    int rValue = m_pRedSlider->getCurrentValue();
    int gValue = m_pGreenSlider->getCurrentValue();
    int bValue = m_pBlueSlider->getCurrentValue();

    QColor color(rValue, gValue, bValue);
    m_pColorCircle->setCurrentColor(color);
    m_pColorView->setCurrentColor(color);

    m_currentColor = color;

    // 更新Color颜色显示
    updateForColorSlider();
    updateForColorSliderValue();
}

void UICustomColorWidget::onHSVSliderColorChanged(void)
{
    int h = m_pHSlider->getCurrentValue();
    int s = m_pSSlider->getCurrentValue();
    int v = m_pVSlider->getCurrentValue();

    QColor color;
    color.setHsv(h, s, v);
    m_pColorCircle->setCurrentColor(color);
    m_pColorView->setCurrentColor(color);

    m_currentColor = color;

    // 更新Color颜色显示
    updateForColorSlider();
    updateForColorSliderValue();
}

void UICustomColorWidget::updateForColorSlider(void)
{
    m_pRedSlider->setStartColor(QColor(0, m_currentColor.green(), m_currentColor.blue()));
    m_pRedSlider->setEndColor(QColor(255, m_currentColor.green(), m_currentColor.blue()));

    m_pGreenSlider->setStartColor(QColor(m_currentColor.red(), 0, m_currentColor.blue()));
    m_pGreenSlider->setEndColor(QColor(m_currentColor.red(), 255, m_currentColor.blue()));

    m_pBlueSlider->setStartColor(QColor(m_currentColor.red(), m_currentColor.green(), 0));
    m_pBlueSlider->setEndColor(QColor(m_currentColor.red(), m_currentColor.green(), 255));

    // s
    int h, s, v;
    m_currentColor.getHsv(&h, &s, &v);
    QColor startColor, endColor;
    startColor.setHsv(h, 0, v);
    endColor.setHsv(h, 255, v);
    m_pSSlider->setStartColor(startColor);
    m_pSSlider->setEndColor(endColor);

    // v
    startColor.setHsv(h, s, 0);
    endColor.setHsv(h, s, 255);
    m_pVSlider->setStartColor(startColor);
    m_pVSlider->setEndColor(endColor);
}

void UICustomColorWidget::updateForColorSliderValue(void)
{
    m_pRedSlider->setCurrentValue(m_currentColor.red());
    m_pGreenSlider->setCurrentValue(m_currentColor.green());
    m_pBlueSlider->setCurrentValue(m_currentColor.blue());

    int h, s, v;
    m_currentColor.getHsv(&h, &s, &v);

    m_pHSlider->setCurrentValue(h);
    m_pSSlider->setCurrentValue(s);
    m_pVSlider->setCurrentValue(v);
}

// 保存和加载
void UICustomColorWidget::saveCustomColor(void)
{
    QList<QColor> colors;
    for (auto iter = m_colorViewsList.begin(); iter != m_colorViewsList.end(); ++iter)
    {
        QColor color = (*iter)->getCurrentColor();
        colors << color;
    }

    m_pCustomColorOperator->saveCustomColors(colors);
}

void UICustomColorWidget::loadCustomColor(void)
{
    QList<QColor> colors;
    m_pCustomColorOperator->loadCustonColors(colors);

    int count = 0;
    for (auto iter = colors.begin(); iter != colors.end(); ++iter)
        m_colorViewsList[count++]->setCurrentColor(*iter);
}

// 设置/获取当前的颜色
void UICustomColorWidget::setCurrentColor(const QColor& color)
{
    m_currentColor = color;

    m_pColorCircle->setCurrentColor(color);
    m_pColorView->setCurrentColor(color);

    // 更新Color颜色显示
    updateForColorSlider();
    updateForColorSliderValue();
}

QColor UICustomColorWidget::getCurrentColor(void)
{
    return m_currentColor;
}
