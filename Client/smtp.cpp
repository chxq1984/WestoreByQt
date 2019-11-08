#include "smtp.h"
#include<qDebug>
#include <QtGlobal>
#include <QTime>
Smtp::Smtp(QByteArray username,QByteArray password)
{
    if(username.contains("@qq"))
    {
        this->username = username;
        this->password = password;
    }
    else
        qDebug()<<"NOT qq";
}

void Smtp::send(QByteArray recvaddr,QString subject,QString content)
{
    this->recvaddr = recvaddr;
    this->subject = subject;
    this->content = content;
    QByteArray usernametmp = this->username;
    QByteArray recvaddrtmp = this->recvaddr;

    clientsocket=new QTcpSocket();
    this->clientsocket->connectToHost("smtp.qq.com",25,QTcpSocket::ReadWrite);
    this->clientsocket->waitForConnected(1000);
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;


    this->clientsocket->write("HELO smtp.qq.com\r\n");
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;

    this->clientsocket->write("AUTH LOGIN\r\n");
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;

    this->clientsocket->write(username.toBase64().append("\r\n"));
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;

    this->clientsocket->write(password.toBase64().append("\r\n"));
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;

    this->clientsocket->write(mailfrom.append(usernametmp.append(">\r\n")));
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;

    this->clientsocket->write(rcptto.append(recvaddrtmp.append(">\r\n")));
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;

    this->clientsocket->write("data\r\n");
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;


    usernametmp = this->username;
    recvaddrtmp = this->recvaddr;

    this->clientsocket->write(prefrom.append(usernametmp.append("\r\n")));
    this->clientsocket->write(preto.append(recvaddrtmp.append("\r\n")));
    this->clientsocket->write(presubject.append(subject.toLocal8Bit().append("\r\n")));
    this->clientsocket->write("\r\n");
    this->clientsocket->write(content.toLocal8Bit().append("\r\n"));
    this->clientsocket->write(".\r\n");
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;

    this->clientsocket->write("quit\r\n");
    this->clientsocket->waitForReadyRead(1000);
    recvdata = clientsocket->readAll();
    qDebug()<<recvdata;
}

Smtp::~Smtp()
{
    delete this->clientsocket;
}
QString Smtp::get_randnumber()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    QString map="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString vcode;
    for(int m=0;m<6;m++)
    {
        int random=qrand()%map.length();
        vcode+=map[random];
    }
    return vcode;
}

