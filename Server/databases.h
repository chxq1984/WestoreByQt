#ifndef DATABASES_H
#define DATABASES_H
#include <QSqlDatabase>//用于连接数据库
#include <QSqlQuery>

class databases
{
public:
    void connect_database();
    databases();
    QSqlQuery *query;
    QSqlDatabase db;
};

#endif // DATABASES_H
