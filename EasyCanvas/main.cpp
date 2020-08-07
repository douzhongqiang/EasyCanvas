#include <QApplication>
#include "UICustomCore/CustomStyleConfig.h"
#include "MainWindow.h"
#include "PythonWrapCore.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //  初始化Python环境
    g_PythonWrapCode->init();
    //  初始化样式和翻译
    g_StyleConfig->init();
    g_StyleConfig->loadLanguage("./CustomNormalCode_zh_CH.qm");

    MainWindow w;
    w.showMaximized();

    return a.exec();
}
