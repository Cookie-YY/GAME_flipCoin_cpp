#include <QDebug>
#include <QPropertyAnimation>

#include "mypushbutton.h"

/*
 *  按钮的封装，第一场景的开始按钮，关卡选择场景的关卡选择按钮，三个场景都用到的back按钮
*/


/* MyPushButton: 构造函数
 *   1. 设置两个图片，设置图片样式(固定大小，去掉边框，加载图标图片)
*/
MyPushButton::MyPushButton(QString normalImg, QString pressImg)
{
    this->normalImgPath = normalImg;
    this->pressImgPath = pressImg;

    QPixmap pix;
    bool ret = pix.load(normalImg);
    if (!ret)
    {
        qDebug() << "图片加载失败";
        return;
    }
    // 设置图片大小: 不让用户改变
    this->setFixedSize(pix.width(), pix.height());

    // 去掉边框: 比如圆形图片可以去掉背景
    this->setStyleSheet("QPushButton{border:0px;}");

    // 设置图标
    this->setIcon(pix);

    // 设置图标大小
    this->setIconSize(QSize(pix.width(), pix.height()));
}


/* pressAnimation: 按压动画: 设置动画: 起点和终点，时间和曲线
*/
void MyPushButton::pressAnimation(int range, int duration)
{
    // 下落效果
    QPropertyAnimation * animation1 = new QPropertyAnimation(this, "geometry");
    // 设置起点和终点
    animation1->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));  // 左上角的坐标， 矩形的大小
    animation1->setEndValue(QRect(this->x(), this->y()+range, this->width(), this->height()));

    // 设置持续时间和曲线
    animation1->setEasingCurve(QEasingCurve::OutBounce);
    animation1->setDuration(duration);  // 200ms
    animation1->start();

    // 弹起效果
    QPropertyAnimation * animation2 = new QPropertyAnimation(this, "geometry");
    // 设置起点和终点
    animation2->setStartValue(QRect(this->x(), this->y()+range, this->width(), this->height()));  // 左上角的坐标， 矩形的大小
    animation2->setEndValue(QRect(this->x(), this->y(), this->width(), this->height()));

    // 设置持续时间和曲线
    animation2->setEasingCurve(QEasingCurve::OutBounce);
    animation2->setDuration(duration);  // 200ms
    animation2->start();
}


/* mousePressEvent: 重写mousePressEvent: 做到按压时更换图片
*/
void MyPushButton::mousePressEvent(QMouseEvent *e)
{
    if (this->pressImgPath != "")
    {
        QPixmap pix;
        bool ret = pix.load(pressImgPath);
        if (!ret)
        {
            qDebug() << "图片加载失败";
            return;
        }
        // 设置图片大小: 不让用户改变
        this->setFixedSize(pix.width(), pix.height());

        // 去掉边框: 比如圆形图片可以去掉背景
        this->setStyleSheet("QPushButton{border:0px;}");

        // 设置图标
        this->setIcon(pix);

        // 设置图标大小
        this->setIconSize(QSize(pix.width(), pix.height()));
    }
    return QPushButton::mousePressEvent(e);
}


/* mouseReleaseEvent: 重写mouseReleaseEvent: 做到释放时把图片换回来
*/
void MyPushButton::mouseReleaseEvent(QMouseEvent *e)
{
    if (this->pressImgPath != "")
    {
        QPixmap pix;
        bool ret = pix.load(normalImgPath);
        if (!ret)
        {
            qDebug() << "图片加载失败";
            return;
        }
        // 设置图片大小: 不让用户改变
        this->setFixedSize(pix.width(), pix.height());

        // 去掉边框: 比如圆形图片可以去掉背景
        this->setStyleSheet("QPushButton{border:0px;}");

        // 设置图标
        this->setIcon(pix);

        // 设置图标大小
        this->setIconSize(QSize(pix.width(), pix.height()));
    }
    return QPushButton::mouseReleaseEvent(e);
}
