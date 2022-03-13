#ifndef LEVELWINDOW_H
#define LEVELWINDOW_H

#include <QMainWindow>
#include "playwindow.h"

class LevelWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit LevelWindow(QWidget *parent = nullptr);

    // 重写paintEvent事件，画背景图
    void paintEvent(QPaintEvent *);

    // 游戏场景的窗口
    PlayWindow * playWindow = NULL;

signals:
    void levelWindowBackToMainWindow();  // 只定义，不实现，使用时直接emit this->levelWindowBackToMainWindow();

public slots:
};

#endif // LEVELWINDOW_H
