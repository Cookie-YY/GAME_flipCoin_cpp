#include <QMenuBar>
#include <QDesktopWidget>
#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QDebug>
#include <QSound>

#include "levelwindow.h"
#include "mypushbutton.h"
#include "playwindow.h"

/*
 *  关卡选择场景搭建，第二场景，包括窗口的跳转与信息的传递
*/


/* LevelWindow: 构造函数
 *   1. 窗口初始化: 大小、标题、标题图标、菜单栏退出按钮
 *        * 选择关卡音效 + 返回按钮音效
 *   2. 主场景搭建: 关卡选择的button + 关卡选择的label
 *        * 跳转的时候携带信息是通过构造函数实现的
 *        * 这里存在文字对齐和事件穿透的问题
 *   3. 下方的返回按钮: 返回按钮的封装
*/
LevelWindow::LevelWindow(QWidget *parent) : QMainWindow(parent)
{
//    // 配置主场景
//    QDesktopWidget* desktopWidget = QApplication::desktop();
//    QRect screenRect = desktopWidget->screenGeometry();
//    this->move(screenRect.width()/2 - this->width()/2,(screenRect.height()/2 - this->height()/2)*7/10 );
    // 1. 窗口初始化
    // 大小
    setFixedSize(320, 588);
    // 图标
    setWindowIcon(QIcon(":/res/Coin0001.png"));
    // 标题
    setWindowTitle("翻金币——by Cookie");
    // 菜单栏的退出按钮
    QMenuBar * bar = menuBar();
    setMenuBar(bar);
    QMenu * startMenu = bar->addMenu("开始");
    QAction * actionquit = startMenu->addAction("退出");
    // 菜单栏退出按钮
    connect(actionquit, &QAction::triggered, [=](){  // mac 在顶端，win在界面上
        this->close();
    });
    // 选择关卡音效
    QSound * levelSound = new QSound(":/res/TapButtonSound.wav", this);  // this让谁放，放到对象树中
    QSound * backSound = new QSound(":/res/BackButtonSound.wav", this);  // this让谁放，放到对象树中

    // 2. 主场景搭建: 关卡选择
    for(int i=0; i<20; i++)
    {
        // 关卡选择的button: 绑定功能
        MyPushButton * levelButton = new MyPushButton(":/res/LevelIcon.png");
        levelButton->setParent(this);
        levelButton->move(25 + i%4*70, 130 + i/4*70);
        connect(levelButton, &MyPushButton::clicked, [=](){
            levelSound->play();
            this->hide();
            // 游戏场景
            playWindow = new PlayWindow(i);
            playWindow->setGeometry(this->geometry());
            playWindow->show();
            connect(playWindow, &PlayWindow::playWindowBackToLevelWindow, [=](){
                this->setGeometry(playWindow->geometry());
                this->show();
                // 每次点击都即时创建，返回时即时销毁
                delete playWindow;
                playWindow = NULL;
            });
        });
        // 关卡选择的label: 显示数字
        QLabel * label = new QLabel;
        label->setParent(levelButton);
        label->setFixedSize(levelButton->width(), levelButton->height());
        label->setText(QString::number(i+1));
        // label的文字对齐
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
    // 返回按钮
    MyPushButton * backButton = new MyPushButton(":/res/BackButton.png", ":/res/BackButtonSelected.png");
    backButton->setParent(this);
    backButton->move(this->width()-backButton->width(), this->height()-backButton->height());
    connect(backButton, &QPushButton::clicked, this, [=](){
        backSound->play();
        QTimer::singleShot(50, [=](){
            emit this->levelWindowBackToMainWindow();
        });
    });

}


/* paintEvent: 重写 paintEvent 事件添加背景图片
*/
void LevelWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    // 加载背景图片
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    // 加载标题图片
    pix.load(":/res/Title.png");
    painter.drawPixmap((this->width() - pix.width()) / 2,30,pix.width(),pix.height(),pix);  // 标题
}
