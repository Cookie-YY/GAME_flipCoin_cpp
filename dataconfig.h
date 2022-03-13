#ifndef DATACONFIG_H
#define DATACONFIG_H

#include <QObject>
#include <QMap>
#include <QVector>

class dataConfig : public QObject
{
    Q_OBJECT
public:
    explicit dataConfig(QObject *parent = 0);

public:

    QMap<int, QVector< QVector<int> > >mData;  // key: 关卡  value: 二维数组保存初始化是金币还是银币 1金币 0银币



signals:

public slots:
};

#endif // DATACONFIG_H
