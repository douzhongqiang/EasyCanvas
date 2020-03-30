#ifndef UICUSTOMGROUPCONTROL_H
#define UICUSTOMGROUPCONTROL_H

#include <QWidget>
#include <QVBoxLayout>
#include "UICustomWidgtes/CustomWidget.h"
#include "customcontrols_global.h"

class UICustomGroupTitleItem;
class UICustomGroupContentItem;

// 组控件
class CUSTOMCONTROLSSHARED_EXPORT UICustomGroupControl : public CustomWidget
{
    Q_OBJECT

public:
    UICustomGroupControl(QWidget* parent = nullptr);
    ~UICustomGroupControl();

    // 添加内容
    void addContentWidget(QWidget* widget);
    void insertContentWidget(int index, QWidget* widget);

    // 设置标题文本
    void setTitleText(const QString& string);

protected:
    virtual QSize sizeHint() const override;

private:
    QVBoxLayout* m_pMainLayout = nullptr;
    UICustomGroupTitleItem* m_pTitleItemWidget = nullptr;
    UICustomGroupContentItem* m_pContentItemWidget = nullptr;

private slots:
    void onClickedGroupTitleItem(bool isExpanded);
};

// -----------------------------------------------------------------------------
// 标题
class UICustomGroupTitleItem : public CustomWidget
{
    Q_OBJECT

public:
    UICustomGroupTitleItem(QWidget* parent = nullptr);
    ~UICustomGroupTitleItem();

    // 设置Title内容
    void setTitleText(const QString& titleNameString);

    friend class UICustomGroupControl;
protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void customPaint(QPainter* painter) override;
    virtual QSize sizeHint() const override;

private:
    QString m_cTitleName;

    bool m_isExpanding = true;

signals:
    void clickedTitle(bool isExpanded);
};

// -----------------------------------------------------------------------------
// 组件内容
class UICustomGroupContentItem : public CustomWidget
{
    Q_OBJECT

public:
    UICustomGroupContentItem(QWidget* parent = nullptr);
    ~UICustomGroupContentItem();

    void addWidget(QWidget* widget);
    void insertWidget(int index, QWidget* widget);

    friend class UICustomGroupControl;
protected:
    virtual QSize sizeHint() const override;

private:
    QVBoxLayout* m_pLayout = nullptr;
};

#endif
