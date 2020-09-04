#include "CustomStyleConfig.h"
#include <QApplication>
#include <QPalette>
#include <QFile>
#include <QStyle>
#include <QCoreApplication>
#include <QTranslator>
#include <QDebug>

CustomStyleConfig::CustomStyleConfig()
    :m_cNormalBgColor(80, 80, 80)
    ,m_cNormalTextColor(255, 255, 255)
    ,m_cNormalHighLightColor(243, 125, 40)
{
    QPalette pal;
    pal.setBrush(QPalette::Background, QBrush(m_cNormalBgColor));
    pal.setBrush(QPalette::Foreground, QBrush(m_cNormalTextColor));

    qApp->setPalette(pal);
}

CustomStyleConfig::~CustomStyleConfig()
{

}

CustomStyleConfig* CustomStyleConfig::getInstance(void)
{
    static CustomStyleConfig instance;
    return &instance;
}

void CustomStyleConfig::init(void)
{
    qDebug() << __FUNCTION__;

    m_sCurrentPropertyName = "LightStyleProperty";
    QString styleStringPath = ":/qss/light.css";

    QFile nFile(styleStringPath);
    nFile.open(QFile::ReadOnly);
    QString styleSheetStr = nFile.readAll();
    nFile.close();

    qApp->setStyleSheet(styleSheetStr);

    // init font
    QFont font;
    font.setPointSizeF(10);
    //font.setPixelSize(24);
    qApp->setFont(font);
}

void CustomStyleConfig::setCurrentStyle(QWidget* widget, const QString& propertyValue, bool needPolish)
{
    widget->setProperty(m_sCurrentPropertyName.toLocal8Bit().data(), propertyValue);
    
    if (needPolish)
    {
        qApp->style()->unpolish(widget);
        qApp->style()->polish(widget);
    }
}

void CustomStyleConfig::loadLanguage(const QString& qmFilePath)
{
    QTranslator *translator = new QTranslator(this);
    bool result = translator->load(qmFilePath);
    if (result)
        qApp->installTranslator(translator);
}

const QColor& CustomStyleConfig::getNormalBackgroundColor(void)
{
    return m_cNormalBgColor;
}

const QColor& CustomStyleConfig::getNormalTextColor(void)
{
    return m_cNormalTextColor;
}

const QColor& CustomStyleConfig::getHighLightColor(void)
{
    return m_cNormalHighLightColor;
}
