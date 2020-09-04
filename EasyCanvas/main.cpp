#include <QApplication>
#include <QDebug>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <QFile>
#include "UICustomCore/CustomStyleConfig.h"
#include "MainWindow.h"
#include "PythonWrapCore.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString messageString = "%1: %2 (%3:%4, %5)\n";
    QString typeString;
    switch (type) {
    case QtDebugMsg:
        typeString = "Debug";
        break;
    case QtInfoMsg:
        typeString = "Info";
        break;
    case QtWarningMsg:
        typeString = "Warning";
        break;
    case QtCriticalMsg:
        typeString = "Critical";
        break;
    case QtFatalMsg:
        typeString = "Fatal";
        break;
    }

    messageString = messageString.arg(typeString).arg(msg).arg(context.file).arg(context.line).arg(context.function);

    QFile file("./log.txt");
    file.open(QFile::Append);
    QTextStream stream(&file);
    stream << messageString;
    file.close();
}

int main(int argc, char *argv[])
{
    //qInstallMessageHandler(myMessageOutput);
    QApplication a(argc, argv);

    qDebug() << "EasyCanvas Starting...";

    //  初始化样式和翻译
    g_StyleConfig->init();
    g_StyleConfig->loadLanguage("./CustomNormalCode_zh_CH.qm");

    qDebug() << "Init Pythoning";
    //  初始化Python环境
    g_PythonWrapCode->init();
    qDebug() << "EasyCanvas Loading Widnows...";

    MainWindow w;
    w.initGlobalHotKey();
    w.showMaximized();

    return a.exec();
}
