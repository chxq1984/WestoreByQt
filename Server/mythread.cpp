#include "mythread.h"

mythread::mythread()
{

}
void mythread::write_ptr(qintptr p)
{
    ptr=p;
}
void mythread::run()
{
    socket=new mysocket;
    socket->setSocketDescriptor(ptr);//客户端的初始化
    if(socket->waitForConnected(10000)){
        qDebug()<<"Connect Success";
    }
    else{
        qDebug()<<"Connect Fail";
    }
    this->exec();
}

