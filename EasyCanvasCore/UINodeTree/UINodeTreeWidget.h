#ifndef UINODETREEWIDGET_H
#define UINODETREEWIDGET_H

#include <QWidget>
#include "easycanvascore_global.h"
class UINodeTreeView;
class QLineEdit;
class UICustomLineEdit;
class EASYCANVASCORESHARED_EXPORT UINodeTreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UINodeTreeWidget(QWidget* parent = nullptr);
    ~UINodeTreeWidget();

private:
    UICustomLineEdit* m_pLineEdit = nullptr;
    UINodeTreeView* m_pTreeView = nullptr;

private slots:
    void onTextChanged(const QString& text);
};

#endif
