#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>
#include <QTimer>

class MyCoin : public QPushButton
{
    Q_OBJECT
public:
//    explicit MyCoin(QWidget *parent = nullptr);
    MyCoin(QString buttonImg);  // 开始时的金币图片路径

    // 金币属性
    int poX; // 第几行
    int poY; // 第几列
    bool flag; // 正反标识

    // 翻转方法
    void flipCoin();
    QTimer * timer1;  // 正->反
    QTimer * timer2;  // 反->正
    int min = 1; // 图片路径的后缀
    int max = 8; // 图片路径的后缀

signals:

public slots:
};

#endif // MYCOIN_H
