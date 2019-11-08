#include "myserver.h"

myserver::myserver()
{

}
void myserver::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"New Connect is connect"<<socketDescriptor;
    mythread * thread=new mythread();
    thread->write_ptr(socketDescriptor);
    thread->start();
}
