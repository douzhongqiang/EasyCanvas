#pragma once

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "customwidgets_global.h"

/**
* Window Title
*/
class CustomDialog;
class CustomDialogTitle : public QWidget
{
    Q_OBJECT
public:
    CustomDialogTitle();
    ~CustomDialogTitle();
    friend class CustomDialog;

private:
    // controls
    QLabel *m_pTitleText = nullptr;
    QPushButton* m_pMinimizeButton = nullptr;
    QPushButton* m_pMaximizeButton = nullptr;
    QPushButton* m_pCloseButton = nullptr;

private:
    int m_nHeight = 30;
    QSize m_buttonSize;

protected:
    void paintEvent(QPaintEvent *event) override;
};

/**
* Windows Styles Dialog
*/
class QVBoxLayout;
class CUSTOMWIDGETSSHARED_EXPORT CustomDialog : public QDialog
{
    Q_OBJECT

public:
    CustomDialog(QWidget *parent = Q_NULLPTR);
    ~CustomDialog();

    //设置是否可以通过鼠标调整窗口大小
    //if resizeable is set to false, then the window can not be resized by mouse
    //but still can be resized programtically
    void setResizeable(bool resizeable = true);
    bool isResizeable(){ return m_bResizeable; }

    // setContentWidget
    void setContentWidget(QWidget* contentWidget);
    // 设置标题
    void setWindowTitle(const QString& str);

public:
    void setContentsMargins(const QMargins &margins);
    void setContentsMargins(int left, int top, int right, int bottom);
    QMargins contentsMargins() const;
    QRect contentsRect() const;
    void getContentsMargins(int *left, int *top, int *right, int *bottom) const;

public slots:
    void showFullScreen();

protected:
    QVBoxLayout *m_pMainLayout = nullptr;

    void paintEvent(QPaintEvent* event) override;
    bool nativeEvent(const QByteArray & eventType, void * message, long * result) override;

private:
    static const int m_nBorderWidth;

    // init ui(init titlebar)
    void initUi(void);
    CustomDialogTitle* m_pTitleWidget = nullptr;

    QVBoxLayout* m_pTotlaLayout = nullptr;

    QMargins m_margins;
    QMargins m_frames;
    bool m_bJustMaximized = false;
    bool m_bResizeable = true;

private slots:
    void onClickedMaximizeButton(void);
    void onWindowStatusChanged(Qt::WindowState windowStatus);
};
