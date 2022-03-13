#include <QDebug>
#include <QTimer>

#include "mycoin.h"

/*
 *  金币的封装，游戏场景用到的金币，涉及动画的效果
*/


/* MyCoin: 构造函数
 *   1. 加载金币图片: 设置图片样式
 *   2. 监听翻转信号(正->反, 反->正)
 *         * 如果翻转完了需要停止循环定时器
*/
MyCoin::MyCoin(QString buttonImg)
{
    // 1. 加载金币图片
    QPixmap pix;
    bool ret = pix.load(buttonImg);
    if (!ret)
    {
        QString str = QString("金币图图片: %1加载失败").arg(buttonImg);
        qDebug() << str;
        return;
    }
    // 设置图片样式
    this->setFixedSize(pix.width(), pix.height());
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(), pix.height()));

    // 2. 监听正面翻反面的信号，翻转金币
    timer1 = new QTimer(this);  // 正->反的定时器，start之后，只要不stop每个给定的时间就发送 timeout信号
    timer2 = new QTimer(this);  // 反->正的定时器，start之后，只要不stop每个给定的时间就发送 timeout信号
    connect(timer1, &QTimer::timeout, [=](){
        // 加载图片
        QPixmap pix;
        QString str = QString(":/res/Coin000%1.png").arg(this->min++);
        pix.load(str);
        // 绘制
        this->setFixedSize(pix.width(), pix.height());
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(), pix.height()));
        // 如果翻转完了 1.停止计时器  2.重置min
        if (this->min > this->max)
        {
            this->min = 1;
            timer1->stop();
        }

    });

    // 监听正面翻反面的信号，翻转金币
    connect(timer2, &QTimer::timeout, [=](){
        // 加载图片
        QPixmap pix;
        QString str = QString(":/res/Coin000%1.png").arg(this->max--);
        pix.load(str);
        // 绘制
        this->setFixedSize(pix.width(), pix.height());
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(), pix.height()));
        // 如果翻转完了 1.停止计时器  2.重置max
        if (this->max < this->min)
        {
            this->max = 8;
            timer2->stop();
        }

    });
}


/* flipCoin: 翻转金币
 *   1. 修改标记位
 *   2. 启动对应的循环定时器
*/
void MyCoin::flipCoin()
{
    if(this->flag)  // 如果this->flag是true
    {
        this->flag=false;
        timer1->start(30);
    }
    else
    {
        this->flag=true;
        timer2->start(30);
    }
}
