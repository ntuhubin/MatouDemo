#include "cmysqlpro.h"
#include <time.h>
#include <QDebug>
#include <stdio.h>


CMySQLPro::CMySQLPro()
{
    mysql_init(&mysql);
}
bool CMySQLPro::ConnectDB()
{
    if (mysql_real_connect(&mysql, "localhost", "root", "pwd123", "facedb", 3306, NULL, 0))
    {
        return true;
    }
    return false;
}
void CMySQLPro::CloseDB()
{
    mysql_close(&mysql);
}
/*uuid_t CMySQLPro::GetGUID()
{
    uuid_t uu;
    uuid_generate(uu);
    return uu;
}*/
void CMySQLPro::InsertTrace(ClsResult result, int camid)
{
    MYSQL_STMT *stmt = mysql_stmt_init(&mysql); //创建MYSQL_STMT句柄
    char *query = "insert into persontrace(personname,capDate,cappic,belief,camid) values(?,?,?,?,?)";
    if(mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        qDebug() << mysql_stmt_error(stmt);
        return;
    }
    string personname = result.lable.toStdString();
    unsigned long namelen = strlen(personname.c_str());
    char *c = new char[namelen];
    strcpy(c,personname.c_str());

    MYSQL_BIND params[5];
    memset(params, 0, sizeof(params));
    int idx = 0;

    params[idx].buffer_type = MYSQL_TYPE_STRING;
    params[idx].buffer = c;
    params[idx].buffer_length = 0;
    params[idx].is_null= 0;
    params[idx].length= &namelen;
    idx++;

    MYSQL_TIME  ts;
    ts.year= result.capDate.date().year();
    ts.month= result.capDate.date().month();
    ts.day= result.capDate.date().day();
    ts.hour= result.capDate.time().hour();
    ts.minute= result.capDate.time().minute();
    ts.second= result.capDate.time().second();

    ts.neg = 0;
    ts.second_part = 0;
    params[idx].buffer_type= MYSQL_TYPE_DATETIME;
    params[idx].buffer= (char *)&ts;
    params[idx].is_null= 0;
    params[idx].length= 0;
    idx++;

    result.dectImg.save("/tmp/tt.jpg");
    unsigned long pl = result.dectImg.byteCount() + 1024;
    char *p = new char[pl];
    FILE *fp = NULL;
    fp = fopen("/tmp/tt.jpg", "r");
    unsigned long count = fread(p, 1, pl, fp);
    fclose(fp);

    //memcpy(p,result.dectImg.bits(),pl);
    params[idx].buffer_type= MYSQL_TYPE_LONG_BLOB;
    params[idx].buffer= p;
    params[idx].is_null= 0;
    params[idx].length= &count;
    idx++;

    params[idx].buffer_type = MYSQL_TYPE_DOUBLE;
    params[idx].buffer = (char *)&(result.belief);
    params[idx].is_null= 0;
    params[idx].length= 0;
    idx++;

    params[idx].buffer_type = MYSQL_TYPE_LONG;
    params[idx].buffer = (char *)&camid;
    params[idx].is_null= 0;
    params[idx].length= 0;

    mysql_stmt_bind_param(stmt, params);
    mysql_stmt_send_long_data(stmt,2,p,pl);
    int ret = mysql_stmt_execute(stmt);           //执行与语句句柄相关的预处理
    if(ret != 0)
    {
        delete c;
        delete p;
        qDebug() << mysql_stmt_error(stmt);
        return;
    }
    mysql_stmt_close(stmt);

    delete c;
    delete p;

}
void CMySQLPro::GetTracePerson(string begintime, string endtime)
{
    reslist.clear();
    std::string str_sqls; // = "select personname,capDate,cappic,belief from persontrace";
    char sqlchar[200];
    sprintf(sqlchar,"select personname,capDate,cappic,belief from persontrace where capDate between '%s' and '%s'",begintime.c_str(),endtime.c_str());
    str_sqls = sqlchar;
    int res = 0;
    const char *p = str_sqls.c_str();
    res = mysql_real_query(&mysql, p, str_sqls.size());
    if (res != 0)
    {
        return;
    }
    // 取出SQL 语句执行的结果集
   MYSQL_RES *ms_res = mysql_store_result(&mysql);
   unsigned long *lengths;
   if (ms_res != NULL)
   {
       unsigned int field_num = mysql_num_fields(ms_res);
       // 每个字段的数据信息
       MYSQL_FIELD* field_info = mysql_fetch_field(ms_res);
       //assert(field_info != NULL);
       MYSQL_ROW row_data = NULL;
       while (1) {
           row_data = mysql_fetch_row(ms_res);    // 取出下一行结果

           if (row_data == NULL)
               break;
           SearchResult result;
           lengths = mysql_fetch_lengths(ms_res);
           result.name = QString(QLatin1String(row_data[0]));
           result.capdate =  QString(QLatin1String(row_data[1]));
           unsigned int imglen = lengths[2];
           unsigned char *img = new unsigned char[imglen];
           //qDebug << row_data[0];
           //qDebug << row_data[1];
           memcpy(img, row_data[2],imglen);
           QImage qimg;
           qimg.loadFromData(img, imglen);
           result.capimg = qimg;

           result.belief =  QString(QLatin1String(row_data[3]));
           //result.capimg.save("./a.jpeg");
           reslist.append(result);
           /*FILE *fp = NULL;
           fp = fopen("./res.jpeg","wb");
           fwrite(img,imglen,1,fp);
           fclose(fp);*/
           delete img;
           //qDebug << row_data[3];
           /*for (int i = 0; i < field_num; ++i)
           {
               if (row_data[i] == NULL)
               {
                    //qDebug<<field_info[i];
               }
           }*/
       }
   }
   mysql_free_result(ms_res);
   ms_res = NULL;
}
int CMySQLPro::InsertReg(RegPeson person)
{
    MYSQL_STMT *stmt = mysql_stmt_init(&mysql); //创建MYSQL_STMT句柄
    char *query = "insert into RegPerson(idRegPerson,PName,RegDate,RegPic) values(?,?,?,?)";
    if(mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        return 1;
    }

    unsigned long namelen = strlen(person.name.c_str());
    char *c = new char[namelen];
    strcpy(c,person.name.c_str());

    unsigned long iddlen = strlen(person.id.c_str());
    char *d = new char[iddlen];
    strcpy(d,person.id.c_str());


    MYSQL_BIND params[4];
    memset(params, 0, sizeof(params));
    int idx = 0;

    params[0].buffer_type = MYSQL_TYPE_VAR_STRING;
    params[0].buffer = d;
    params[0].buffer_length = 0;
    params[0].is_null= 0;
    //unsigned long idlen = 16;
    params[0].length= &iddlen;
    idx++;

    params[idx].buffer_type = MYSQL_TYPE_VAR_STRING;
    params[idx].buffer = c;
    params[idx].buffer_length = 0;
    params[idx].is_null= 0;
    params[idx].length= &namelen;
    idx++;

    MYSQL_TIME  ts;
    ts.year= 2018;
    ts.month= 5;
    ts.day= 3;
    ts.hour= 10;
    ts.minute= 45;
    ts.second= 20;
    params[idx].buffer_type= MYSQL_TYPE_DATETIME;
    params[idx].buffer= (char *)&ts;
    params[idx].is_null= 0;
    params[idx].length= 0;
    idx++;

    params[idx].buffer_type= MYSQL_TYPE_MEDIUM_BLOB;
    params[idx].buffer= person.pic;
    params[idx].is_null= 0;
    unsigned long pl = person.piclen;
    params[idx].length= &pl;
    idx++;

    mysql_stmt_bind_param(stmt, params);
    mysql_stmt_send_long_data(stmt,3,person.pic,pl);
    int ret = mysql_stmt_execute(stmt);           //执行与语句句柄相关的预处理
    mysql_stmt_close(stmt);
    delete c;
    delete d;
}
void CMySQLPro::InsertRegPerson(char *img, unsigned long len)
{
    MYSQL_STMT *stmt = mysql_stmt_init(&mysql); //创建MYSQL_STMT句柄
    char *query = "insert into tta(idtta,tb,timg,imglength) values(?,?,?,?)";
    if(mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        return;
    }

    MYSQL_BIND params[4];
    memset(params, 0, sizeof(params));
    int  int_data = 56;
    params[0].buffer_type = MYSQL_TYPE_LONG;
    params[0].buffer = (char *)&int_data;
    params[0].is_null= 0;
    params[0].length= 0;

    MYSQL_TIME  ts;
    ts.year= 2018;
    ts.month= 5;
    ts.day= 2;
    ts.hour= 10;
    ts.minute= 45;
    ts.second= 20;
    params[1].buffer_type= MYSQL_TYPE_DATETIME;
    params[1].buffer= (char *)&ts;
    params[1].is_null= 0;
    params[1].length= 0;

    params[2].buffer_type= MYSQL_TYPE_MEDIUM_BLOB;
    params[2].buffer= img;
    params[2].is_null= 0;
    params[2].length= &len;

    params[3].buffer_type= MYSQL_TYPE_LONG;
    params[3].buffer= (char *)&len;;
    params[3].is_null= 0;
    params[3].length= 0;


    mysql_stmt_bind_param(stmt, params);
    mysql_stmt_send_long_data(stmt,2,img,len);
    int ret = mysql_stmt_execute(stmt);           //执行与语句句柄相关的预处理
    mysql_stmt_close(stmt);

}
void CMySQLPro::ReadPerson(int id)
{
    std::string str_sqls = "select * from tta where idtta=56";
    int res = 0;
    const char *p = str_sqls.c_str();
    res = mysql_real_query(&mysql, p, str_sqls.size());
    if (res != 0)
    {
        return;
    }
    // 取出SQL 语句执行的结果集
   MYSQL_RES *ms_res = mysql_store_result(&mysql);
   unsigned long *lengths;
   if (ms_res != NULL)
   {
       unsigned int field_num = mysql_num_fields(ms_res);
       // 每个字段的数据信息
       MYSQL_FIELD* field_info = mysql_fetch_field(ms_res);
       //assert(field_info != NULL);
       MYSQL_ROW row_data = NULL;
       while (1) {
           row_data = mysql_fetch_row(ms_res);    // 取出下一行结果

           if (row_data == NULL)
               break;
           lengths = mysql_fetch_lengths(ms_res);
           int imglen = lengths[2];
           char *img = new char[imglen];
           //qDebug << row_data[0];
           //qDebug << row_data[1];
           memcpy(img, row_data[2],imglen);
           FILE *fp = NULL;
           fp = fopen("/home/hb/res.jpeg","wb");
           fwrite(img,imglen,1,fp);
           fclose(fp);
           //qDebug << row_data[3];
           for (int i = 0; i < field_num; ++i)
           {
               if (row_data[i] == NULL)
               {
                    //qDebug<<field_info[i];
               }
           }
       }
   }
   mysql_free_result(ms_res);
   ms_res = NULL;
}
