#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "levelwindow.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 重写paintEvent事件，画背景图
    void paintEvent(QPaintEvent *);

    // 第二场景的窗口(关卡选择的窗口)
    LevelWindow * levelWindow = NULL;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
