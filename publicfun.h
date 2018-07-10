#ifndef PUBLICFUN_H
#define PUBLICFUN_H
#include <QtCore>
#include <QtGui>
#include <QList>
#include <QDir>
#include <QFile>
class publicFun: public QObject
{
public:
    static QList<QString> GetImagePath()
    {
        QString filepath = "./img";
        QStringList filters;
        filters<<QString("*.jpeg")<<QString("*.jpg")<<QString("*.png")<<QString("*.tiff")<<QString("*.gif")<<QString("*.bmp");
        QDir d(filepath);
        d.setFilter( QDir::NoSymLinks | QDir::Files);
        d.setNameFilters(filters);
        return d.entryList();
    }
};
#endif // PUBLICFUN_H
