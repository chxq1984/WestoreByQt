#ifndef CHANGE_NAME_H
#define CHANGE_NAME_H

#include <QDialog>
#include "globals.h"
#include <QTcpSocket>
#include "tcp.h"
namespace Ui {
class change_name;
}

class change_name : public QDialog
{
    Q_OBJECT

public:
    explicit change_name(QWidget *parent = nullptr);
    ~change_name();

private slots:
    void on_button_ok_clicked();
    void ReadData();
    //void ReadError(QAbstractSocket::SocketError);
private:
    Ui::change_name *ui;
    //QTcpSocket*tcpClient;
    TCP *communicate;
    void SolveCommand(QString command);
};

#endif // CHANGE_NAME_H
