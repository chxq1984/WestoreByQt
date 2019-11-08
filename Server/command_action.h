#ifndef COMMAND_ACTION_H
#define COMMAND_ACTION_H
#include <QVector>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <cmath>
#include "command_explain.h"
#include "global.h"
class command_action
{
public:
    command_action();
    void decoding(QString frombuffer);//传入QString类型的指令码将其解码至command_set中
    QVector<QString> command_set;
    virtual QString action()=0;//声明为虚函数，对不同操作分类进行,返回需要发送给客户端的内容，没有则返回空
    void connect_mysql();
    QSqlDatabase db;
    QSqlQuery *query;
    command_explain *explain;
};
class action_register:public command_action
{
private:
    QString action();
};
class acton_shopping_list:public command_action
{
private:
    QString action();
};
class action_friend_list:public command_action
{
private:
    QString action();
};
class acton_shopping_car:public command_action
{
private:
    QString action();
};
class action_collection:public command_action
{
private:
    QString action(); 
};
class action_comment:public command_action
{
private:
    QString action();
};

#endif // COMMAND_ACTION_H
