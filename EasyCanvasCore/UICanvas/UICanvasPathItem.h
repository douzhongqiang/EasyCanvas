#ifndef UICANVASPATHITEM_H
#define UICANVASPATHITEM_H

#include "UICanvasItemBase.h"
#include <QPainterPath>
#include <QGraphicsItem>

class NDIntAttribute;
class NDColorAttribute;
class UICanvasPathItem : public UICanvasItemBase
{
    Q_OBJECT

public:
    UICanvasPathItem(QGraphicsItem* parentItem = nullptr);
    ~UICanvasPathItem();

    // 添加点
    void addPoint(const QPointF& scenePos);
    // 设置起始点
    void setStartPos(const QPointF& startScenePos);
    // 结束设置
    void endDrawPos(void);

    // 获取当前属性Json
    virtual QJsonObject getStoreJson(int interval = 0) override;
    virtual void fillJsonInfo(const QJsonObject& jsonObject) override;

protected:
    void customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    //QPainterPath shape() const override;

private:
    bool m_hasPoints = false;
    bool m_hadAddPointFinished = false;
    QPointF m_painterStartPos;

    QPainterPath m_painterPath;
    QVector<QPointF> m_points;

    // 初始化
    void initAttribute(void);
    NDIntAttribute* m_pOutLineWidthAttribute = nullptr;             // 轮廓线宽度
    NDColorAttribute* m_pOutLineColorAttribute = nullptr;           // 轮廓线颜色

private slots:
    void onValueChanged(void);
};

#endif
