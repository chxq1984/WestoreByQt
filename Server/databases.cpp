#include "databases.h"
#include <QSqlDatabase>//用于连接数据库
#include <qDebug>
#include <QTime>
#include <QRandomGenerator>
databases::databases()
{

}
void databases::connect_database()
{
    quint32 value = QRandomGenerator::global()->generate();//

    if ( true == QSqlDatabase::contains(QString::number(value)))
    {
        db = QSqlDatabase::database(QString::number(value));
    }
    else
    {
        db = QSqlDatabase::addDatabase("QMYSQL",QString::number(value));
    }
    db.setHostName("localhost");
    db.setDatabaseName("WeStore");
    db.setUserName("root");
    db.setPassword("453480049");
    if (!db.open())
        qDebug() << "Failed to connect to root mysql admin";
    else
        qDebug() << "open";
    query=new QSqlQuery(db);

}
