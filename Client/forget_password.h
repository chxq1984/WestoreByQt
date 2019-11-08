#ifndef FORGET_PASSWORD_H
#define FORGET_PASSWORD_H

#include <QDialog>
#include <QTcpSocket>
#include "smtp.h"
#include "tcp.h"
namespace Ui {
class forget_password;
}

class forget_password : public QDialog
{
    Q_OBJECT

public:
    explicit forget_password(QWidget *parent = nullptr);
    ~forget_password();
    QString vcode;
private slots:
    void ReadData();
    //void ReadError(QAbstractSocket::SocketError);
    void on_button_getvcode_clicked();

    void on_button_ok_clicked();

private:
    Ui::forget_password *ui;
    //QTcpSocket*tcpClient;
    Smtp *email;
    QString position_email;
    TCP*communicate;
    void SolveCommand(QString command);
    int check();
};

#endif // FORGET_PASSWORD_H
