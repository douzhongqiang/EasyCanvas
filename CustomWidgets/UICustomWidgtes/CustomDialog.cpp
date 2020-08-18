#include "CustomDialog.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QPalette>
#include <QStyleOption>
#include <QWindow>
#include <QDebug>

#include <windowsx.h>
#include <qt_windows.h>
#include <dwmapi.h>

#include "UICustomCore/CustomStyleConfig.h"
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "user32.lib")

CustomDialogTitle::CustomDialogTitle() 
    :m_buttonSize(20, 20)
{
    // init Style
    g_StyleConfig->setCurrentStyle(this, "DefaultTitleBar");

    this->setFixedHeight(m_nHeight);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(20, 0, 20, 0);
    mainLayout->setSpacing(20);

    // Title
    m_pTitleText = new QLabel;
    m_pTitleText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(m_pTitleText);

    // minimize Button
    m_pMinimizeButton = new QPushButton;
    m_pMinimizeButton->setObjectName("minButton");
    m_pMinimizeButton->setFixedSize(m_buttonSize);
    m_pMinimizeButton->setDefault(false);
    mainLayout->addWidget(m_pMinimizeButton);

    // maximize Button
    m_pMaximizeButton = new QPushButton;
    g_StyleConfig->setCurrentStyle(m_pMaximizeButton, "maxNormal");
    m_pMaximizeButton->setObjectName("maxButton");
    m_pMaximizeButton->setFixedSize(m_buttonSize);
    m_pMaximizeButton->setDefault(false);
    mainLayout->addWidget(m_pMaximizeButton);

    // close Button
    m_pCloseButton = new QPushButton;
    m_pCloseButton->setObjectName("closeButton");
    m_pCloseButton->setFixedSize(m_buttonSize);
    m_pCloseButton->setDefault(false);
    mainLayout->addWidget(m_pCloseButton);
}

CustomDialogTitle::~CustomDialogTitle()
{

}

void CustomDialogTitle::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    return QWidget::paintEvent(event);
}

// -----------------------------------------------------------------------
const int CustomDialog::m_nBorderWidth = 10;

CustomDialog::CustomDialog(QWidget *parent)
    : QDialog(parent)
{
    Qt::WindowFlags windowFlags = this->windowFlags() | Qt::Window | Qt::FramelessWindowHint;
    windowFlags &= ~Qt::WindowSystemMenuHint;

    setWindowFlags(windowFlags);

    // init UI
    initUi();
}

CustomDialog::~CustomDialog()
{

}

void CustomDialog::setResizeable(bool resizeable)
{
    bool visible = isVisible();
    m_bResizeable = resizeable;
    if (m_bResizeable){
        setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
        //        setWindowFlag(Qt::WindowMaximizeButtonHint);

        //此行代码可以带回Aero效果，同时也带回了标题栏和边框,在nativeEvent()会再次去掉标题栏
        //
        //this line will get titlebar/thick frame/Aero back, which is exactly what we want
        //we will get rid of titlebar and thick frame again in nativeEvent() later
        HWND hwnd = (HWND)this->winId();
        DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
        ::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);
    }
    else{
        setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
        //        setWindowFlag(Qt::WindowMaximizeButtonHint,false);

        HWND hwnd = (HWND)this->winId();
        DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
        ::SetWindowLong(hwnd, GWL_STYLE, style & ~WS_MAXIMIZEBOX & ~WS_CAPTION);
    }

    //保留一个像素的边框宽度，否则系统不会绘制边框阴影
    //
    //we better left 1 piexl width of border untouch, so OS can draw nice shadow around it
    const MARGINS shadow = { 1, 1, 1, 1 };
    ::DwmExtendFrameIntoClientArea(HWND(winId()), &shadow);

    setVisible(visible);
}

void CustomDialog::setContentWidget(QWidget* contentWidget)
{
    if (contentWidget == nullptr)
        return;

    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pMainLayout->addWidget(contentWidget);
}

void CustomDialog::setWindowTitle(const QString& str)
{
    this->m_pTitleWidget->m_pTitleText->setText(str);
    QDialog::setWindowTitle(str);
}

void CustomDialog::setContentsMargins(const QMargins &margins)
{
    QDialog::setContentsMargins(margins + m_frames);
    m_margins = margins;
}

void CustomDialog::setContentsMargins(int left, int top, int right, int bottom)
{
    QDialog::setContentsMargins(left + m_frames.left(), \
        top + m_frames.top(), \
        right + m_frames.right(), \
        bottom + m_frames.bottom());
    m_margins.setLeft(left);
    m_margins.setTop(top);
    m_margins.setRight(right);
    m_margins.setBottom(bottom);
}

QMargins CustomDialog::contentsMargins() const
{
    QMargins margins = QDialog::contentsMargins();
    margins -= m_frames;
    return margins;
}

QRect CustomDialog::contentsRect() const
{
    QRect rect = QDialog::contentsRect();
    int width = rect.width();
    int height = rect.height();
    rect.setLeft(rect.left() - m_frames.left());
    rect.setTop(rect.top() - m_frames.top());
    rect.setWidth(width);
    rect.setHeight(height);
    return rect;
}

void CustomDialog::getContentsMargins(int *left, int *top, int *right, int *bottom) const
{
    QDialog::getContentsMargins(left, top, right, bottom);
    if (!(left&&top&&right&&bottom)) return;
    if (isMaximized())
    {
        *left -= m_frames.left();
        *top -= m_frames.top();
        *right -= m_frames.right();
        *bottom -= m_frames.bottom();
    }
}

void CustomDialog::showFullScreen()
{
    if (isMaximized())
    {
        QDialog::setContentsMargins(m_margins);
        m_frames = QMargins();
    }
    QDialog::showFullScreen();
}

void CustomDialog::initUi(void)
{
    this->setContentsMargins(2, 2, 2, 2);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    m_pTotlaLayout = mainLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    
    // title
    m_pTitleWidget = new CustomDialogTitle;
    m_pTitleWidget->m_pTitleText->setText("My Test Title");
    mainLayout->addWidget(m_pTitleWidget);

    // content
    QWidget* pContentWidget = new QWidget;
    pContentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(pContentWidget);

    // Content Layout
    m_pMainLayout = new QVBoxLayout(pContentWidget);
    m_pMainLayout->setMargin(0);
    m_pMainLayout->setSpacing(0);

    QObject::connect(m_pTitleWidget->m_pMinimizeButton, &QPushButton::clicked, this, &CustomDialog::showMinimized);
    QObject::connect(m_pTitleWidget->m_pMaximizeButton, &QPushButton::clicked, this, &CustomDialog::onClickedMaximizeButton);
    QObject::connect(m_pTitleWidget->m_pCloseButton, &QPushButton::clicked, this, &CustomDialog::close);
    
    if (this->windowHandle())
        QObject::connect(this->windowHandle(), &QWindow::windowStateChanged, this, &CustomDialog::onWindowStatusChanged);
}

void CustomDialog::onClickedMaximizeButton(void)
{
    if (this->isMaximized())
        this->showNormal();
    else
        this->showMaximized();
}

void CustomDialog::onWindowStatusChanged(Qt::WindowState windowStatus)
{
    bool isMaxed = false;
    if (windowStatus == Qt::WindowMaximized)
    {
        isMaxed = true;
        //m_pTotlaLayout->setContentsMargins(10, 10, 1, 1);
        g_StyleConfig->setCurrentStyle(m_pTitleWidget->m_pMaximizeButton, "maxed", true);
    }
    else if (windowStatus == Qt::WindowNoState)
    {
        g_StyleConfig->setCurrentStyle(m_pTitleWidget->m_pMaximizeButton, "maxNormal", true);
    }
}

void CustomDialog::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QPen nPen;
    nPen.setWidth(2);
    nPen.setColor(QColor(0, 0, 0));
    p.setPen(nPen);
    p.drawRect(this->rect());

    return QDialog::paintEvent(event);
}

bool CustomDialog::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG *param = static_cast<MSG *>(message);

    switch (param->message)
    {
    case WM_NCCALCSIZE: {
        NCCALCSIZE_PARAMS& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(param->lParam);
        if (params.rgrc[0].top != 0)
                params.rgrc[0].top -= 1;
        //this kills the window frame and title bar we added with WS_THICKFRAME and WS_CAPTION
        *result = WVR_REDRAW;
        return true;
    }
    case WM_NCHITTEST:
    {
        const LONG border_width = m_nBorderWidth;
        RECT winrect;
        GetWindowRect(HWND(winId()), &winrect);

        long x = GET_X_LPARAM(param->lParam);
        long y = GET_Y_LPARAM(param->lParam);
        int nX = x - this->geometry().x();
        int nY = y - this->geometry().y();

        //support highdpi
        double dpr = this->devicePixelRatioF();
        if (this->childAt(mapFromGlobal(QPoint(x/dpr,y/dpr))) == m_pTitleWidget->m_pTitleText)
        {
            *result = HTCAPTION;
            return true;
        }

        QRect nRect = this->rect().adjusted(m_nBorderWidth, m_nBorderWidth, -m_nBorderWidth, -m_nBorderWidth);
        bool needDispose = this->rect().contains(nX, nY) && !nRect.contains(nX, nY);
        if (!needDispose)
            return QDialog::nativeEvent(eventType, message, result);

        // dispose window frame
        if ((nX > 0) && (nX < m_nBorderWidth))
            *result = HTLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width()))
            *result = HTRIGHT;

        if ((nY > 0) && (nY < m_nBorderWidth))
            *result = HTTOP;

        if ((nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOM;

        if ((nX > 0) && (nX < m_nBorderWidth) && (nY > 0)
            && (nY < m_nBorderWidth))
            *result = HTTOPLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
            && (nY > 0) && (nY < m_nBorderWidth))
            *result = HTTOPRIGHT;

        if ((nX > 0) && (nX < m_nBorderWidth)
            && (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOMLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
            && (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOMRIGHT;

        return true;
    }
    case WM_GETMINMAXINFO:
    {
        if (::IsZoomed(param->hwnd)) 
        {
            RECT frame = { 0, 0, 0, 0 };
            AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);

            //record frame area data
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
            double dpr = this->devicePixelRatioF();
#else
            double dpr = 1.0;
#endif

            m_frames.setLeft(abs(frame.left) / dpr + 0.5);
            m_frames.setTop(abs(frame.bottom) / dpr + 0.5);
            m_frames.setRight(abs(frame.right) / dpr + 0.5);
            m_frames.setBottom(abs(frame.bottom) / dpr + 0.5);

            QDialog::setContentsMargins(m_frames.left() + m_margins.left(), \
                m_frames.top() + m_margins.top(), \
                m_frames.right() + m_margins.right(), \
                m_frames.bottom() + m_margins.bottom());
            m_bJustMaximized = true;
            g_StyleConfig->setCurrentStyle(m_pTitleWidget->m_pMaximizeButton, "maxed", true);
        }
        else 
        {
            if (m_bJustMaximized)
            {
                QDialog::setContentsMargins(m_margins);
                m_frames = QMargins();
                m_bJustMaximized = false;
                g_StyleConfig->setCurrentStyle(m_pTitleWidget->m_pMaximizeButton, "maxNormal", true);
            }
        }
        return false;
    }
    }

    return QDialog::nativeEvent(eventType, message, result);
}

void CustomDialog::showEvent(QShowEvent* event)
{
    if (!m_isShowed)
    {
        m_isShowed = true;
        setResizeable(m_bResizeable);
    }

    QDialog::showEvent(event);
}

