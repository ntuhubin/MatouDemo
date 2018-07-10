#ifndef PLAYIMGTHREAD_H
#define PLAYIMGTHREAD_H
#include <QThread>
#include <QList>
#include <QPainter>
#include "publicfun.h"

class PlayImgThread:public QThread
{
    Q_OBJECT
signals:
    void message(QImage img);
public:
    PlayImgThread();
private:
    volatile bool b_stop;
    QList<QString> imgpathlist;
private:
    void run();
};

#endif // PLAYIMGTHREAD_H
