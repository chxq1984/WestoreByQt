#include "tcp.h"

TCP::TCP(QObject *parent) : QObject(parent)
{
    Date="";
}
void TCP::SetUp()
{
    tcpClient = new QTcpSocket(this);   //实例化tcpClient
    tcpClient->abort();                 //取消原有连接
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), \
                this, SLOT(ReadError(QAbstractSocket::SocketError)));
}

void TCP::ReadError(QAbstractSocket::SocketError)
{
    tcpClient->disconnectFromHost();
    QMessageBox msgBox;
    msgBox.setText("信息错误");
    msgBox.exec();
}
void TCP::ConnectToHost(QString Ip, int Port)
{
    tcpClient->connectToHost(Ip, Port);
    tcpClient->waitForConnected(1000);
}
void TCP::SendDate(QString data)
{
    if(data != "")
    {
        //bool a=tcpClient->write(data.toLatin1()); //QString转QByteArray
        bool a=tcpClient->write(data.toUtf8());
    }
}
void TCP::DisConnect()
{
    tcpClient->disconnectFromHost();
    if (tcpClient->state() == QAbstractSocket::UnconnectedState \
        || tcpClient->waitForDisconnected(1000))  //已断开连接则进入if{}
    {
        //
    }
}
QString TCP::ReturnDate()
{
    return Date;
}
