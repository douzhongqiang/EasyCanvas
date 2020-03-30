#pragma once

#include <QMainWindow>

class CustomMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CustomMainWindow(QWidget *parent = Q_NULLPTR);
    ~CustomMainWindow();
};
