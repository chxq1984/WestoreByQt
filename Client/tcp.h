#ifndef TCP_H
#define TCP_H

#include <QObject>
#include <QTcpSocket>

#include <QMessageBox>
class TCP : public QObject
{
    Q_OBJECT
public:
    explicit TCP(QObject *parent = nullptr);
    void SetUp();
    void ConnectToHost(QString Ip,int Port);
    void SendDate(QString data);
    void DisConnect();
    QString ReturnDate();
    QTcpSocket*tcpClient;
signals:

public slots:
    void ReadError(QAbstractSocket::SocketError);
private:
    QString Date;


};

#endif // TCP_H
