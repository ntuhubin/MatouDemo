#ifndef G_H
#define G_H
#include <QDate>
#include <QRectF>
#include <QImage>

typedef struct
{
    QString name;
    QImage img;
    int label;
    int realcount;
}SaveRegPerson, *pSaveRegPerson;
typedef struct
{
    QString name;
    QList<QString> imgs;
    QList<float> features;  //128*sizeof(imgs)
    int lable;
}RegisterPerson, *pRegisterPerson;
typedef struct
{
    QString name;
    QImage regImg;    //zhuce zhao pian
    QImage capImg;     //zhua tu
    QDateTime capDate;     //zhua tu shijian
    int label;
    double belief;
}ShowRegPerson, *pShowRegPerson;
typedef struct
{
    QString label;
    double belief;
    double pos[4];
    QList<float> features;  //128*sizeof(imgs)
}ClsInfo, *pClsInfo;
typedef struct
{
    QImage dectImg;
    QList<float> features;  //128*sizeof(imgs)
    QString lable;   // 实际是注册人姓名
    double belief;
    QDateTime capDate;
}ClsResult,*pClsResult;

typedef struct
{
    std::string name;
    QList<float> features;  //128
    int showtimes;
    QDateTime Lastshow;
    QImage dectimg;
}StrangerInfo, *pStrangerInfo;

//QList<SaveRegPerson> srList;



#endif // G_H
