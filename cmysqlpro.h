#ifndef CMYSQLPRO_H
#define CMYSQLPRO_H
#include <mysql/mysql.h>
#include <string>
#include <string.h>
#include <time.h>
//#include <uuid/uuid.h>
#include "g.h"
#include <QDebug>
using namespace std;
struct RegPeson
{
     string id;
     string name;
     time_t timep;
     char *pic;
     int piclen;
};
typedef struct
{
    QString name;
    QString belief;
    QImage capimg;
    QString capdate;
}SearchResult, *pSearchResult;
class CMySQLPro
{
private:
    MYSQL mysql;
    MYSQL_RES *result;
    MYSQL_ROW sql_row;
public:
    QList<SearchResult> reslist;
public:
    CMySQLPro();
    //uuid_t GetGUID();
    bool ConnectDB();
    void CloseDB();
    int InsertReg(RegPeson person);
    void InsertRegPerson(char *img, unsigned long len);
    void ReadPerson(int id);
    void GetTracePerson(string begintime, string endtime);
    void InsertTrace(ClsResult result, int camid);
};

#endif // CMYSQLPRO_H
