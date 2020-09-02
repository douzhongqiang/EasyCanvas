#ifndef UICANVASSCENE_H
#define UICANVASSCENE_H

#include <QGraphicsScene>
class NDNodeBase;
class NDIntAttribute;
class NDColorAttribute;
class UICanvasScene : public QGraphicsScene
{
    Q_OBJECT

public:
    UICanvasScene(QObject* parent = nullptr);
    ~UICanvasScene();

    // 获取节点
    NDNodeBase* getCurrentNode(void);
    // 初始化节点属性
    void resetNodeInfo(void);

protected:
    virtual void drawBackground(QPainter *painter, const QRectF &rect) override;
    virtual void drawForeground(QPainter *painter, const QRectF &rect) override;

private:
    // 初始化属性
    void initNodeInfo(void);

    // 属性
    NDNodeBase* m_pNode = nullptr;
    // 宽度和高度属性
    NDIntAttribute* m_pWidthAttribute = nullptr;
    NDIntAttribute* m_pHeightAttribute = nullptr;
    // 颜色属性
    NDColorAttribute* m_pBackgroundColorAttribute = nullptr;

private slots:
    void onWidthAttributeValueChanged(const QVariant& value);
    void onHeightAttributeValueChanged(const QVariant& value);
    void onColorAttributeValueChanged(const QVariant& value);
};

#endif
