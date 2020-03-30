#include "UIAboutMeDialog.h"

UIAboutMeDialog::UIAboutMeDialog(QWidget* parent)
    :CustomDialog(parent)
{
    m_pImageViewer = new UICustomImageViewer;
    m_pImageViewer->loadImage("./AboutMe.png");

    this->setWindowTitle(tr("About Me"));
    this->setContentWidget(m_pImageViewer);
}

UIAboutMeDialog::~UIAboutMeDialog()
{

}
