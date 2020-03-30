#include <QApplication>
#include "UICustomCore/CustomStyleConfig.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    g_StyleConfig->init();
    g_StyleConfig->loadLanguage("./CustomNormalCode_zh_CH.qm");

    MainWindow w;
    w.showMaximized();

    return a.exec();
}
