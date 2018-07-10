#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "playimgthread.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void MonitorStart();
    void MonitorExit();
private slots:
    void recvImg(QImage img);
private:
    Ui::Widget *ui;
    PlayImgThread *play_thread;
};

#endif // WIDGET_H
