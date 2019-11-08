#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include "mysocket.h"
class mythread:public QThread
{
public:
    mythread();
    qintptr ptr;
    mysocket * socket;
    void write_ptr(qintptr p);
protected:
    void run();
};

#endif // MYTHREAD_H
