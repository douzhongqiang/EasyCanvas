#include "UIAboutMeDialog.h"
#include <QApplication>

UIAboutMeDialog::UIAboutMeDialog(QWidget* parent)
    :CustomDialog(parent)
{
    m_pImageViewer = new UICustomImageViewer;
    QString curAppPath = qApp->applicationDirPath() + "/";
    m_pImageViewer->loadImage(curAppPath + "./AboutMe.png");

    this->setWindowTitle(tr("About Me"));
    this->setContentWidget(m_pImageViewer);
}

UIAboutMeDialog::~UIAboutMeDialog()
{

}
