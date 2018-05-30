#ifndef PLAYIMGTHREAD_H
#define PLAYIMGTHREAD_H
#include <QThread>

class PlayImgThread:public QThread
{
    Q_OBJECT
signals:
    void message();
public:
    PlayImgThread();
private:
    volatile bool b_stop;
private:
    void run();
};

#endif // PLAYIMGTHREAD_H
