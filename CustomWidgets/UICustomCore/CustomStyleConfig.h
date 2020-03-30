#ifndef CUSTOMSTYLECONFIG_H
#define CUSTOMSTYLECONFIG_H

#include <QObject>
#include <QWidget>
#include "customwidgets_global.h"
#define g_StyleConfig CustomStyleConfig::getInstance()

class CUSTOMWIDGETSSHARED_EXPORT CustomStyleConfig : public QObject
{
public:
    static CustomStyleConfig* getInstance(void);
    
    void init(void);
    void setCurrentStyle(QWidget* widget, const QString& propertyValue, bool needPolish = false);

    // 加载语言
    void loadLanguage(const QString& qmFilePath);

    // 获取颜色
    const QColor& getNormalBackgroundColor(void);
    const QColor& getNormalTextColor(void);
    const QColor& getHighLightColor(void);

private:
    CustomStyleConfig();
    ~CustomStyleConfig();

    QString m_sCurrentPropertyName;
    QColor m_cNormalBgColor;
    QColor m_cNormalTextColor;
    QColor m_cNormalHighLightColor;
};

#endif
