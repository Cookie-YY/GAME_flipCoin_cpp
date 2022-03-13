#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
//    explicit MyPushButton(QWidget *parent = nullptr);
    MyPushButton(QString normalImg, QString pressImg="");

    QString normalImgPath;
    QString pressImgPath;

    // 动画效果
    void pressAnimation(int range=10, int duration=200);

    // 图片切换效果
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);


signals:

public slots:
};

#endif // MYPUSHBUTTON_H
