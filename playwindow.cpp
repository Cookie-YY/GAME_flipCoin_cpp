#include <QMenuBar>
#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QtDebug>
#include <QPropertyAnimation>
#include <QSound>

#include "playwindow.h"
#include "mypushbutton.h"
#include "mycoin.h"
#include "dataconfig.h"

/*
 *  游戏场景搭建，涉及翻转金币的核心逻辑
*/


/* PlayWindow: 构造函数
 *   1. 窗口初始化: 大小、标题、标题图标、菜单栏退出按钮
 *   2. 加载资源: 音效 + 胜利图片(等待胜利后砸下来)
 *   3. 保存基本信息: 当前关卡，初始化的金币还是银币位置
 *   4. 主场景搭建: 16个灰色背景框 + 16个初始金币(并赋值属性) + 1个存放金币的二维数组
 *        * 遍历二维数组，监听所有的金币点击事件
 *   5. 下方提示与返回: label的字体运用(字体字号大小)  +  返回按钮的封装
*/
PlayWindow::PlayWindow(int levelIndex)
{
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

    // 2. 加载资源
    // 加载音效
    QSound * backSound = new QSound(":/res/BackButtonSound.wav", this);  // this让谁放，放到对象树中
    QSound * flipSound = new QSound(":/res/ConFlipSound.wav", this);  // this让谁放，放到对象树中
    this->winSound = new QSound(":/res/LevelWinSound.wav", this);  // this让谁放，放到对象树中
    // 加载胜利图片
    this->winPic = new QLabel;
    QPixmap winPix;
    winPix.load(":/res/LevelCompletedDialogBg.png");
    this->winPic->setGeometry(0,0,winPix.width(), winPix.height());
    this->winPic->setPixmap(winPix);
    this->winPic->setParent(this);
    this->winPic->move((this->width()-winPix.width())/2, -winPix.height());  // -winPix.height()在上面移出屏幕外

    // 3. 保存基本信息
    // 保存当前关卡索引
    this->levelIndex = levelIndex;
    // 保存初始化数组
    dataConfig config;
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<4; j++)
        {
            this->gameArray[i][j] = config.mData[this->levelIndex+1][i][j];
        }
    }

    // 4. 主场景搭建
    for(int i=0; i<4; i++)
    {
        for (int j=0; j<4; j++)
        {
            // label背景
            QPixmap pix = QPixmap(":/res/BoardNode(1).png");
            QLabel * label = new QLabel;
            label->setParent(this);

            label->setGeometry(0,0,pix.width(), pix.height());
            label->setPixmap(pix);
            label->move(57 + i*pix.width(), 200+j*pix.height());

            // 初始币图片
            QString imgPath;
            if (this->gameArray[i][j] == 1)
            {
                imgPath = ":/res/Coin0001.png";
            }
            else
            {
                imgPath = ":/res/Coin0008.png";
            }
            MyCoin * coin = new MyCoin(imgPath);
            coin->setParent(label);
//            coin->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            coin->move(2,3);  // 相对于label移动

            // 给金币赋值属性
            coin->poX = i;
            coin->poY = j;
            coin->flag = this->gameArray[i][j]; // 直接使用隐式类型转换  1 true  0 false
            this->coinArray[i][j] = coin;
        }
    }
    // 监听所有币
    for (int i = 0; i<16; i++)
    {
        connect(coinArray[i%4][i/4], &QPushButton::clicked, [=](){
            if (!this->isWin) {
                flipSound->play();
                this->flipCoins(i%4, i/4);  // 翻转当前币及其周围的币，翻转完进行胜利检测
            }
        });
    }

    // 5. 下方提示与返回
    // 关卡显示
    QLabel * label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QString str = QString("Level: %1").arg(this->levelIndex+1);
    label->setFont(font);
    label->setText(str);
    label->setGeometry(30, this->height()-50, 120,50);
    // 返回按钮
    MyPushButton * backButton = new MyPushButton(":/res/BackButton.png", ":/res/BackButtonSelected.png");
    backButton->setParent(this);
    backButton->move(this->width()-backButton->width(), this->height()-backButton->height());
    connect(backButton, &QPushButton::clicked, this, [=](){
        backSound->play();
        QTimer::singleShot(50, [=](){
            emit this->playWindowBackToLevelWindow();
        });
    });

}

/* flipCoins: 游戏场景中翻转金币
 *   1. coinArray中找到对应位置的金币进行翻转
 *   2. 维护 gameArray 的翻转结果(判定是否胜利)
 *   3. 延时翻转其余金币
*/
void PlayWindow::flipCoins(int poX, int poY)
{
    MyCoin * coin = coinArray[poX][poY];
    coin->flipCoin();
    // 维护ganmeArray数组
    this->gameArray[poX][poY] = this->gameArray[poX][poY] == 1?0:1;

    // 延时翻转其他位置
    QTimer::singleShot(200, [=](){
        int x;
        int y;
        if (coin->poX+1<=3) // 翻转右侧
        {
            x = coin->poX+1;
            y = coin->poY;
            this->coinArray[x][y]->flipCoin();
            this->gameArray[x][y] = this->gameArray[x][y] == 1?0:1;
        }
        if (coin->poX-1>=0)  // 翻转左侧
        {
            x = coin->poX-1;
            y = coin->poY;
            this->coinArray[x][y]->flipCoin();
            this->gameArray[x][y] = this->gameArray[x][y] == 1?0:1;
        }
        if (coin->poY+1<=3)  // 翻转下侧
        {
            x = coin->poX;
            y = coin->poY+1;
            this->coinArray[x][y]->flipCoin();
            this->gameArray[x][y] = this->gameArray[x][y] == 1?0:1;
        }
        if (coin->poY-1>=0)  // 翻转上侧
        {
            x = coin->poX;
            y = coin->poY-1;
            this->coinArray[x][y]->flipCoin();
            this->gameArray[x][y] = this->gameArray[x][y] == 1?0:1;
        }
        this->checkWinAndDoWin();  // 检测是否胜利，如果胜利，执行之后的操作
    });

}


/* checkWinAndDoWin: 检测是否胜利
 *   1. 遍历gameArray，只要有反面就return
 *   2. 都是正面将 isWin 设置为true
 *   3. 将胜利图片砸下来
*/
void PlayWindow::checkWinAndDoWin()
{
    for (int i=0; i<16; i++)
    {
        if (this->gameArray[i%4][i/4]==0)  // 有反面
        {
            return;
        }
    }

    // 将 isWin 设置为true
    this->isWin = true;
    this->winSound->play();
    // 将胜利图片砸下来
    QPropertyAnimation * animation1 = new QPropertyAnimation(this->winPic, "geometry");
    // 设置起点和终点
    animation1->setStartValue(QRect(this->winPic->x(), this->winPic->y(), this->winPic->width(), this->winPic->height()));  // 左上角的坐标， 矩形的大小
    animation1->setEndValue(QRect(this->winPic->x(), this->winPic->y()+114, this->winPic->width(), this->winPic->height()));

    // 设置持续时间和曲线
    animation1->setEasingCurve(QEasingCurve::OutBounce);
    animation1->setDuration(1000);  // 200ms
    animation1->start();
}


/* paintEvent: 重写 paintEvent 事件添加背景图片
*/
void PlayWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    // 加载背景图片
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    // 加载标题图片
    pix.load(":/res/Title.png");
    painter.drawPixmap(10,30,pix.width(),pix.height(),pix);  // 标题
}

