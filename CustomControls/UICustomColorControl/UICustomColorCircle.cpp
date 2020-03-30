#include "UICustomColorCircle.h"
#include "UICustomCore/CustomStyleConfig.h"
#include <QPainter>
#include <QMouseEvent>
#include <QLinearGradient>
#include <math.h>

UICustomColorCircle::UICustomColorCircle(QWidget* parent)
    :CustomWidget(parent)
{
    m_colorVec << QColor(255, 0, 0) << QColor(255, 255, 0) \
               << QColor(0, 255, 0) << QColor(0, 255, 255) \
               << QColor(0, 0, 255) << QColor(255, 0, 255);

    //this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

UICustomColorCircle::~UICustomColorCircle()
{

}

void UICustomColorCircle::customPaint(QPainter* painter)
{
    painter->setRenderHint(QPainter::Antialiasing);

    QConicalGradient conicalGradient(this->rect().center(), 0);
    qreal interval = 1.0 / m_colorVec.size();
    // 设置渐变颜色
    for (int i=0; i<m_colorVec.size(); ++i)
    {
        conicalGradient.setColorAt(interval * i, m_colorVec[i]);
    }
    conicalGradient.setColorAt(1.0, m_colorVec[0]);

    // 绘制圆环
    painter->setBrush(conicalGradient);
    painter->setPen(QPen(Qt::NoPen));
    int width = qMin(this->width(), this->height()) / 2 * 2;
    QRect centerRect(this->rect().center().x() - width / 2, \
                     this->rect().center().y() - width / 2, \
                     width, width);
    painter->drawEllipse(centerRect);

    // 绘制中间部分
    centerRect.adjust(m_nCircleWidth, m_nCircleWidth, -m_nCircleWidth, -m_nCircleWidth);
    painter->setBrush(QColor(g_StyleConfig->getNormalBackgroundColor()));
    painter->drawEllipse(centerRect);

    // 绘制H选择圆圈
    QPen nPen;
    nPen.setColor(QColor(0, 0, 0));
    nPen.setWidth(2);
    painter->setPen(nPen);
    painter->setBrush(Qt::NoBrush);
    QVector2D vec(cos(m_nHValue / (180 / 3.1415926)), sin(m_nHValue / (180 / 3.1415926)));
    vec.normalize();
    int length = centerRect.width() / 2 + m_nCircleWidth / 2;
    vec = vec * length;
    vec = QVector2D(this->rect().center().x() + vec.x(), this->rect().center().y() - vec.y());
    painter->drawEllipse(QPoint(vec.x(), vec.y()), m_nCircleWidth / 5, m_nCircleWidth / 5);

    // 绘制中间矩形
    int d = static_cast<int>(sqrt(2) * centerRect.width() / 2);
    QRect rect(centerRect.center().x() - d / 2, centerRect.center().y() - d / 2, d, d);
    rect.adjust(2, 2, -2, -2);
    drawCenterRect(rect, painter);
}

void UICustomColorCircle::drawCenterRect(const QRect& rect, QPainter* painter)
{
    painter->save();
    QLinearGradient linearGradientH(rect.topLeft(), rect.topRight());
    linearGradientH.setColorAt(0, QColor(255, 255, 255));
    QColor color;
    color.setHsv(m_nHValue, 255, 255);
    linearGradientH.setColorAt(1, color);
    painter->fillRect(rect, linearGradientH);

    QLinearGradient linearGradientV(rect.topLeft(), rect.bottomLeft());
    linearGradientV.setColorAt(0, QColor(0, 0, 0, 0));
    linearGradientV.setColorAt(1, QColor(0, 0, 0, 255));
    painter->fillRect(rect, linearGradientV);

    // 绘制选中的圈
    int h, s, v;
    m_currentColor.getHsv(&h, &s, &v);
    qreal interValX = rect.width() * s * 1.0 / 255;
    qreal interValY = rect.height() * v * 1.0 / 255;
    QPoint centerPos(rect.bottomLeft().x() + interValX, rect.bottomLeft().y() - interValY);
    QPen nPen;
    nPen.setColor(QColor(0, 0, 0));
    nPen.setWidth(2);
    painter->setPen(nPen);
    painter->setBrush(Qt::NoBrush);
    painter->drawEllipse(centerPos, m_nCircleWidth / 5, m_nCircleWidth / 5);

    painter->restore();
}

qreal UICustomColorCircle::distance(QPoint p1, QPoint p2)
{
    return sqrt((p1.x() - p2.x()) * (p1.x() - p2.x()) + (p1.y() - p2.y()) * (p1.y() - p2.y()));
}

int UICustomColorCircle::getHValue(QPoint pos)
{
    QPoint centerPos = this->rect().center();
    QVector2D vec = QVector2D(pos) - QVector2D(centerPos);
    vec.normalize();

    float angle = acos(vec.x());
    int value = static_cast<int>(angle * (180 / 3.1415926));
    if (pos.y() > centerPos.y())
        value = 360 - value;

    return value;
}

void UICustomColorCircle::updateCurrentColor(QPoint pos)
{
    int width = qMin(this->width(), this->height()) / 2 * 2;
    QRect centerRect(this->rect().center().x() - width / 2, \
                     this->rect().center().y() - width / 2, \
                     width, width);

    centerRect.adjust(m_nCircleWidth, m_nCircleWidth, -m_nCircleWidth, -m_nCircleWidth);
    int d = static_cast<int>(sqrt(2) * centerRect.width() / 2);
    QRect rect(centerRect.center().x() - d / 2, centerRect.center().y() - d / 2, d, d);
    rect.adjust(2, 2, -2, -2);

    int sValue = qMin(static_cast<int>((pos.x() - rect.x()) * 1.0 / rect.width() * 255), 255);
    int vValue = qMin(static_cast<int>((rect.bottom() - pos.y()) * 1.0 / rect.height() * 255), 255);
    sValue = qMax(sValue, 0);
    vValue = qMax(vValue, 0);
    m_currentColor.setHsv(m_nHValue, sValue, vValue);
}

QColor UICustomColorCircle::getCurrentColor(void)
{
    int h, s, v;
    m_currentColor.getHsv(&h, &s, &v);
    m_currentColor.setHsv(m_nHValue, s, v);

    return m_currentColor;
}

void UICustomColorCircle::setCurrentColor(const QColor& color)
{
    m_currentColor = color;

    int h, s, v;
    m_currentColor.getHsv(&h, &s, &v);
    m_nHValue = h;
    this->update();
}

void UICustomColorCircle::resizeEvent(QResizeEvent* event)
{
    int width = qMin(this->size().width(), this->size().height());
    m_nCircleWidth = static_cast<int>(width / 8.0);

    return CustomWidget::resizeEvent(event);
}

void UICustomColorCircle::mousePressEvent(QMouseEvent* event)
{
    m_selectMode = Normal;
    QPoint pos = event->pos();

    int width = qMin(this->width(), this->height()) / 2 * 2;
    QRect centerRect(this->rect().center().x() - width / 2, \
                     this->rect().center().y() - width / 2, \
                     width, width);

    qreal distance1 = distance(this->rect().center(), pos);
    qreal distance2 = abs(centerRect.topLeft().x() - this->rect().center().x());

    // 判断是否选中圆环
    centerRect.adjust(m_nCircleWidth, m_nCircleWidth, -m_nCircleWidth, -m_nCircleWidth);
    qreal distance3 = abs(centerRect.topLeft().x() - this->rect().center().x());
    if (distance1 > distance3 && distance1 < distance2)
        m_selectMode = Circle;

    // 判断是否选中中间矩形
    int d = static_cast<int>(sqrt(2) * centerRect.width() / 2);
    QRect rect(centerRect.center().x() - d / 2, centerRect.center().y() - d / 2, d, d);
    rect.adjust(2, 2, -2, -2);
    if (rect.contains(pos))
        m_selectMode = Rect;

    if (m_selectMode == Circle)
    {
        m_nHValue = getHValue(pos);
        emit colorChanged();
        this->update();
    }
    else if (m_selectMode == Rect)
    {
        updateCurrentColor(pos);
        emit colorChanged();
        this->update();
    }

    return CustomWidget::mousePressEvent(event);
}

void UICustomColorCircle::mouseReleaseEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();
    if (m_selectMode == Circle)
    {
        m_nHValue = getHValue(pos);
        emit colorChanged();
        this->update();
    }
    else if (m_selectMode == Rect)
    {
        updateCurrentColor(pos);
        emit colorChanged();
        this->update();
    }

    m_selectMode = Normal;
    return CustomWidget::mouseReleaseEvent(event);
}

void UICustomColorCircle::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();
    if (m_selectMode == Circle)
    {
        m_nHValue = getHValue(pos);
        emit colorChanged();
        this->update();
    }
    else if (m_selectMode == Rect)
    {
        updateCurrentColor(pos);
        emit colorChanged();
        this->update();
    }

    return CustomWidget::mouseMoveEvent(event);
}

QSize UICustomColorCircle::sizeHint() const
{
    QSize size(250, 250);
    return size;
}
