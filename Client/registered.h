#ifndef REGISTERED_H
#define REGISTERED_H

#include <QDialog>
#include "smtp.h"
#include <QTcpSocket>
#include <QStandardItem>
#include <QStandardItemModel>
#include "tcp.h"
namespace Ui {
class registered;
}

class registered : public QDialog
{
    Q_OBJECT

public:
    explicit registered(QWidget *parent = nullptr);
    ~registered();

private slots:
    void on_button_getcode_clicked();

    void on_button_ok_clicked();
    void ReadData();
    //void ReadError(QAbstractSocket::SocketError);
private:
    Ui::registered *ui;
    Smtp *email;
    QString vcode;
    //QTcpSocket*tcpClient;
    QString position_email;
    QString account;
    QString password;
    TCP *communicate;
    void SolveCommand(QString command);
    int checkText();
};

#endif // REGISTERED_H
