#include <QTimer>
#include <QDesktopWidget>
#include <QPainter>
#include <QSound>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mypushbutton.h"
#include "levelwindow.h"

/*
 *  主场景搭建，第一场景，包括窗口的跳转
*/


/* MainWindow: 构造函数
 *   1. 窗口初始化: 大小、标题、标题图标、菜单栏退出按钮  通过ui文件实现
 *      * 准备开始场景音效
 *   2. 主场景搭建: 开始按钮的弹跳动画 + 新窗口的跳转
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 1. 配置主场景
    // 大小
    setFixedSize(320, 588);
    // 图标
    setWindowIcon(QIcon(":/res/Coin0001.png"));
    // 标题
    setWindowTitle("翻金币——by Cookie");
    // 退出按钮
    connect(ui->actionquit, &QAction::triggered, [=](){  // mac 在顶端，win在界面上
        this->close();
    });
    // 开始音效
    QSound * startSound = new QSound(":/res/TapButtonSound.wav", this);  // this让谁放，放到对象树中

    // 2. 主场景搭建: 开始按钮
    MyPushButton * startButton = new MyPushButton(":/res/MenuSceneStartButton.png");
    startButton->setParent(this);
    startButton->move(this->width()/2-startButton->width()/2, this->height() / 10*7);
    // 点击开始按钮的动画
    levelWindow = new LevelWindow;
    // 接收从关卡选择场景(第二场景)返回的信号
    connect(levelWindow, &LevelWindow::levelWindowBackToMainWindow, [=](){
        levelWindow->hide();
        this->setGeometry(levelWindow->geometry());
        this->show();
    });
    connect(startButton, &MyPushButton::clicked, [=](){
        startSound->play();
        startButton->pressAnimation();  // 弹跳效果
        // 延时进入关卡选择场景
        QTimer::singleShot(100, this, [=](){
            this->hide();
            levelWindow->setGeometry(this->geometry());
            levelWindow->show();
        });

    });
}


/* paintEvent: 重写 paintEvent 事件添加背景图片
*/
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    // 加载背景图片
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0, this->width(), this->height(), pix);  // 拉伸画背景
    // 加载标题图片
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width() / 2, pix.height() / 2);
    painter.drawPixmap(30,10,pix);  // 画标题
}


MainWindow::~MainWindow()
{
    delete ui;
}
