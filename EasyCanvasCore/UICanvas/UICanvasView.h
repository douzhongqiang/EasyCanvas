#ifndef UICANVASVIEW_H
#define UICANVASVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSharedPointer>
#include "easycanvascore_global.h"

class UICanvasOperBase;
class UICanvasPathItem;
class NDNodeBase;
class UICanvasScene;
class UICanvasItemBase;
class EASYCANVASCORESHARED_EXPORT UICanvasView : public QGraphicsView
{
    Q_OBJECT

public:
    enum CanvasMode
    {
        t_ArrowMode,            // 选择编辑模式
        t_FreeDrawMode,         // 自由绘制模式
        t_ImageMode,            // 图片模式
    };

public:
    UICanvasView(QWidget* parent = nullptr);
    ~UICanvasView();

    // 设置操作器
    void setCurrentOperator(UICanvasOperBase* canvasOper);

    // Create Items
    void createImageItem(void);
    void createTextItem(void);
    void createRectItem(void);
    void createEllipseItem(void);
    void createAudioItem(void);

    // 设置选中矩形
    void setSelectedRectVisible(bool isVisible);
    // 设置选中矩形范围
    void setSelectedRect(const QRect& rect);

    // 设置/获取当前模式
    void setCurrentMode(CanvasMode mode);
    CanvasMode getCurrentMode(void);

    // 获取当前选中的Item的节点
    NDNodeBase* getCurrentSelectedNode(void);
    QList<NDNodeBase*> getCurrentSelectedNodes(void);
    QList<UICanvasItemBase*> getCurrentSelectedItems(void);
    // 获取当前场景根节点
    NDNodeBase* getCurrentSceneNode(void);
    // 重置场景根节点
    void resetSceneNode(void);

    // 获取中心点坐标
    QPointF getCenterPos(void);

    // 添加到场景中
    void addToScene(UICanvasItemBase* item);
    // 从场景中删除
    void removeFromScene(UICanvasItemBase* item);

    // 保存
    void saveToImage(const QString& imagePath);
    void saveToImage(QImage& saveImage);

    // 清除所有选择
    void cleanAllSelected(void);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

    void drawBackground(QPainter * painter, const QRectF & rect) override;
    void drawForeground(QPainter * painter, const QRectF & rect) override;

private:
    UICanvasScene* m_pScene = nullptr;

    UICanvasPathItem* m_pathItem = nullptr;
    QSharedPointer<UICanvasOperBase> m_pCurrentOper;

    // 选中矩形相关
    bool m_isSelectedRectVisible = false;
    QRect m_selectedRect;
    QColor m_cSelectedPenColor;
    QColor m_cSelectedBrushColor;

    CanvasMode m_mode = t_ArrowMode;
    QPixmap m_penPixmap;

signals:
    void itemSelectedChanged(void);
};

#endif
