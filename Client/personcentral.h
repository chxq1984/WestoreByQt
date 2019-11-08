#ifndef PERSONCENTRAL_H
#define PERSONCENTRAL_H

#include <QMainWindow>
#include "change_name.h"
#include "globals.h"
#include <QTcpSocket>
#include <QMessageBox>
#include "command_explain.h"
#include "moneyin.h"
#include "tcp.h"
#include "friendlist.h"
#include "colletion.h"
#include "shoppingcar.h"
namespace Ui {
class personcentral;
}

class personcentral : public QMainWindow
{
    Q_OBJECT

public:
    explicit personcentral(QWidget *parent = nullptr);
    ~personcentral();
private slots:
    void on_button_changename_clicked();
    void ReadData();
    //void ReadError(QAbstractSocket::SocketError);
    void on_button_inmoney_clicked();

    void on_action_2_triggered();

    void on_action_triggered();

    void on_action_3_triggered();

private:
    Ui::personcentral *ui;
    change_name *change_names;
    //QTcpSocket*tcpClient;
    command_explain *coma;
    moneyin *money_in;
    TCP *communicate;
    void SolveCommand(QString command);
    friendlist *friendlish_show;
    colletion *showcollect;
    shoppingcar *showshoppingcar;
};

#endif // PERSONCENTRAL_H
