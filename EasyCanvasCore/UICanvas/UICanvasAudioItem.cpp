#include "UICanvasAudioItem.h"
#include <QPainter>
#include <QLinearGradient>
#include "Utils.h"
#include "NDStringAttribute.h"
#include "NDColorAttribute.h"
#include "NDNodeBase.h"
#include "AudioPlayer.h"
#include <functional>
#include <QFileDialog>
#include <QGraphicsSceneMouseEvent>
#include "Utils.h"

UICanvasAudioItem::UICanvasAudioItem(QGraphicsItem* parentItem)
    :UICanvasItemBase(parentItem)
    ,m_cStartColor(150, 255, 200)
    ,m_cEndColor(46, 216, 130)
{
    setCurrentSize(QSize(300, 60));
    initAttributes();

    m_pAudioPlayer = new AudioPlayer(this);
    QObject::connect(m_pAudioPlayer, &AudioPlayer::playFinished, \
                     this, &UICanvasAudioItem::onAudioPlayFinished);
}

UICanvasAudioItem::~UICanvasAudioItem()
{

}

void UICanvasAudioItem::setCurrentSize(const QSize& size)
{
    m_size = size;

    this->setItemResizeable(true);
    this->setItemResizeRatio(true, m_size.width() * 1.0 / size.height());
    this->updateAttribute();
}

void UICanvasAudioItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QRectF rect = this->getCustomRect();
    qreal interval = rect.height() / 4.0;
    QPointF centerPos(rect.topLeft().x() + interval * 2, rect.topLeft().y() + interval * 2);

    QPointF localPos = event->pos();
    if (g_utilTool->getDistance(centerPos, localPos) < interval && m_isOpenSuccessed)
    {
        // 点击播放
        if (m_pAudioPlayer->getCurrentPlayStatus() != AudioPlayer::t_playing)
            m_pAudioPlayer->play();
        else
            m_pAudioPlayer->stop();

        this->update();
    }
    else {
        return UICanvasItemBase::mousePressEvent(event);
    }
}

void UICanvasAudioItem::customPaint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    QRectF rect = this->getCustomRect();

    QLinearGradient lineGradient(rect.topLeft(), rect.topRight());
    lineGradient.setColorAt(0, m_cStartColor);
    lineGradient.setColorAt(1.0, m_cEndColor);
    painter->setBrush(lineGradient);

    QPen pen;
    pen.setWidth(1);
    pen.setColor(QColor(200, 200, 200));
    painter->setPen(pen);

    qreal radius = rect.height() / 2.0;
    painter->drawRoundedRect(rect, radius, radius);

    // 绘制中间的圆圈
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255, 255, 255));
    qreal interval = rect.height() / 4.0;
    QPointF centerPos(rect.topLeft().x() + interval * 2, rect.topLeft().y() + interval * 2);
    painter->drawEllipse(centerPos, interval, interval);

    // 绘制播放状态
    if (!isPlaying())
    {
        // 绘制播放的三角形
        QPolygonF polyon;
        polyon << QPointF(centerPos.x() - interval / 2 * cos(PI / 3), centerPos.y() - interval / 2 * sin(PI / 3));
        polyon << QPointF(centerPos.x() - interval / 2 * cos(PI / 3), centerPos.y() + interval / 2 * sin(PI / 3));
        polyon << QPointF(centerPos.x() + interval / 2, centerPos.y());
        painter->setBrush(m_cStartColor);
        painter->drawPolygon(polyon);
    }
    else
    {
        // 绘制正方形
        qreal nWidth = interval * cos(PI / 6);
        QRectF palyRectF(centerPos.x() - nWidth / 2, centerPos.y() - nWidth / 2, nWidth, nWidth);
        painter->setBrush(m_cStartColor);
        painter->drawRect(palyRectF);
    }

    // 绘制时间
    QFont font = painter->font();
    font.setPixelSize(interval * 2);
    painter->setFont(font);
    pen.setColor(QColor(255, 255, 255));
    painter->setPen(pen);
    QString timeString = "%1:%2\"";
    timeString = timeString.arg(m_nTotlaAudioTime / 1000 / 60, 2, 10, QChar('0'))\
            .arg(m_nTotlaAudioTime / 1000 % 60, 2, 10, QChar('0'));
    int width = painter->fontMetrics().width(timeString);
    QRectF timeRectF(rect.right() - width - interval, rect.top() + interval, \
                      width, interval * 2);
    QTextOption o;
    painter->drawText(timeRectF, timeString);

    painter->restore();
}

void UICanvasAudioItem::initAttributes(void)
{
    QString otherAttrs = tr("Other Attribute");
    m_pNode->addAttributeGroup(otherAttrs);

    // 音频文件路径
    m_pFilePathAttribute = new NDStringAttribute;
    m_pFilePathAttribute->setValue("Test Value");
    m_pFilePathAttribute->setDisplayName(tr("Audio File Path: "));
    m_pFilePathAttribute->setName("path");
    m_pFilePathAttribute->setShowButton(true);
    m_pFilePathAttribute->setButtonString(tr("Browse"));
    m_pFilePathAttribute->setButtonFunction(std::bind(&UICanvasAudioItem::showFileBroseDialo, this, \
                                                      std::placeholders::_1));
    m_pNode->addAttribute(otherAttrs, m_pFilePathAttribute);

    // 颜色属性
    // 开始颜色
    m_pStartColorAttribute = new NDColorAttribute;
    m_pStartColorAttribute->setValue(m_cStartColor);
    m_pStartColorAttribute->setDisplayName(tr("Start Color: "));
    m_pStartColorAttribute->setName("startColor");
    m_pNode->addAttribute(otherAttrs, m_pStartColorAttribute);
    // 终止颜色
    m_pEndColorAttribute = new NDColorAttribute;
    m_pEndColorAttribute->setValue(m_cEndColor);
    m_pEndColorAttribute->setDisplayName(tr("End Color: "));
    m_pEndColorAttribute->setName("endColor");
    m_pNode->addAttribute(otherAttrs, m_pEndColorAttribute);

    // 连接信号和槽函数
    QObject::connect(m_pStartColorAttribute, &NDColorAttribute::valueChanged, this, &UICanvasAudioItem::onStartColorValueChanged);
    QObject::connect(m_pEndColorAttribute, &NDColorAttribute::valueChanged, this, &UICanvasAudioItem::onEndColorValueChanged);
    QObject::connect(m_pFilePathAttribute, &NDStringAttribute::valueChanged, this, &UICanvasAudioItem::onTextChanged);
}

bool UICanvasAudioItem::showFileBroseDialo(QString& str)
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open Audio File"), "./", \
                                 tr("Audio Files (*.wav *.mp3)"));
    if (fileName.isEmpty())
        return false;

    str = fileName;
    return true;
}

bool UICanvasAudioItem::isPlaying(void)
{
    if (m_pAudioPlayer == nullptr || !m_isOpenSuccessed)
        return false;

    if (m_pAudioPlayer->getCurrentPlayStatus() == AudioPlayer::t_playing)
        return true;

    return false;
}

void UICanvasAudioItem::onStartColorValueChanged(const QVariant& color)
{
    m_cStartColor = color.value<QColor>();
    this->update();
}

void UICanvasAudioItem::onEndColorValueChanged(const QVariant& color)
{
    m_cEndColor = color.value<QColor>();
    this->update();
}

void UICanvasAudioItem::onTextChanged(const QVariant& fileName)
{
    m_pAudioPlayer->stop();
    m_pAudioPlayer->close();
    m_isOpenSuccessed = m_pAudioPlayer->openAudioFile(fileName.toString());
    if (!m_isOpenSuccessed)
        return;

    AudioPlayer::AudioInfo info = m_pAudioPlayer->getInfo();
    m_nTotlaAudioTime = info.totalTime;
}

void UICanvasAudioItem::onAudioPlayFinished(void)
{
    this->update();
}
