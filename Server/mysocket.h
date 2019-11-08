#ifndef MYSOCKET_H
#define MYSOCKET_H
#include <QTcpSocket>
#include "command_action.h"
#include <QTcpServer>
class mysocket:public QTcpSocket
{
    Q_OBJECT
public:
    mysocket();
public slots:
    void recvData(void);
    void choose_action();
private:
    int m_sockDesc;
    QString frombuffer;
    command_action *coma;
    QString current_account;
};

#endif // MYSOCKET_H
