#include "playimgthread.h"

PlayImgThread::PlayImgThread():QThread()
{
    b_stop = false;
    imgpathlist = publicFun::GetImagePath();
}
void PlayImgThread::run()
{
    QPolygon pts;
    pts.setPoints(4, 856,400, 1046,400,1080,900,840,900);
    QList<QString>::const_iterator it = imgpathlist.begin();
    while(it != imgpathlist.end())
    {
        QString str = "./img/" + *it;
        QImage img = QImage(str);
        if(!img.isNull())
        {
            QPainter painter(&img);
            painter.setPen(QPen(Qt::blue, 4, Qt::DashLine));
            painter.drawConvexPolygon(pts);
            //painter.drawRect(856,400,200, 500);
            emit message(img);
        }
        it++;
        msleep(100);
    }
}
