#ifndef UIABOUITMEDIALOG_H
#define UIABOUITMEDIALOG_H

#include <QWidget>
#include "UICustomWidgtes/CustomDialog.h"
#include "UICustomSingleControl/UICustomImageViewer.h"

class UIAboutMeDialog : public CustomDialog
{
    Q_OBJECT

public:
    UIAboutMeDialog(QWidget* parent = nullptr);
    ~UIAboutMeDialog();

private:
    UICustomImageViewer* m_pImageViewer = nullptr;
};

#endif
