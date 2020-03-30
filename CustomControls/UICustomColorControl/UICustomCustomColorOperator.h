#ifndef UICUSTOMCUSTOMCOLOROPERATOR_H
#define UICUSTOMCUSTOMCOLOROPERATOR_H

#include <QObject>
#include <QDomDocument>
#include <QString>
#include <QColor>

class UICustomCustomColorOperator : public QObject
{
    Q_OBJECT

public:
    UICustomCustomColorOperator(QObject* parent = nullptr);
    ~UICustomCustomColorOperator();

    // 保存和加载颜色
    void saveCustomColors(const QList<QColor>& colors);
    void loadCustonColors(QList<QColor>& colors);

private:
    static const QString m_cFileName;
};

#endif
