#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSound>

#include "mycoin.h"

class PlayWindow : public QMainWindow
{
    Q_OBJECT
public:
//    explicit PlayWindow(QWidget *parent = nullptr);
    PlayWindow(int levelIndex);
    int levelIndex;
    bool isWin=false;  // 如果胜利，禁用掉按钮
    QLabel * winPic;      // 胜利的图片，checkWinAndDoWin会把砸下来
    QSound * winSound; // 胜利的音效

    // 重写paintEvent事件，画背景图
    void paintEvent(QPaintEvent *);

    // 维护当前关卡的初始化数据
    int gameArray[4][4];
    MyCoin * coinArray[4][4];

    // 翻转金币(包括周围的金币)
    void flipCoins(int poX, int poY);
    // 检测是否胜利，胜利之后进行相应操作
    void checkWinAndDoWin();

signals:
    void playWindowBackToLevelWindow();  // 只定义，不实现，使用时直接emit this->playWindowBackToLevelWindow();
public slots:
};

#endif // PLAYWINDOW_H
