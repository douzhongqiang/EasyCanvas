#ifndef UICANVASAUDIOITEM_H
#define UICANVASAUDIOITEM_H

#include "UICanvasItemBase.h"
#include <QGraphicsItem>

class NDStringAttribute;
class NDColorAttribute;
class AudioPlayer;
class UICanvasAudioItem : public UICanvasItemBase
{
    Q_OBJECT

public:
    UICanvasAudioItem(QGraphicsItem* parentItem = nullptr);
    ~UICanvasAudioItem();

    void setCurrentSize(const QSize& size);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QColor m_cStartColor;
    QColor m_cEndColor;
    int m_nTotlaAudioTime = 0;  // ms

    NDStringAttribute* m_pFilePathAttribute = nullptr;      // 文件路径
    NDColorAttribute* m_pStartColorAttribute = nullptr;     // 开始颜色
    NDColorAttribute* m_pEndColorAttribute = nullptr;       // 结束颜色
    void initAttributes(void);

    // 设置音频文件
    bool showFileBroseDialo(QString& str);
    AudioPlayer* m_pAudioPlayer = nullptr;
    bool m_isOpenSuccessed = false;
    // 判断当前是否在播放
    bool isPlaying(void);

private slots:
    void onStartColorValueChanged(const QVariant& color);
    void onEndColorValueChanged(const QVariant& color);
    void onTextChanged(const QVariant& fileName);

    void onAudioPlayFinished(void);
};

#endif
