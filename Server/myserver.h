#ifndef MYSERVER_H
#define MYSERVER_H
#include <QTcpSocket>
#include <QTcpServer>
#include "mythread.h"
class myserver:public QTcpServer
{
public:
    myserver();
protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // MYSERVER_H
