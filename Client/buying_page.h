#ifndef BUYING_PAGE_H
#define BUYING_PAGE_H

#include <QDialog>
#include <QTcpSocket>
#include <QMessageBox>
#include "globals.h"
#include "tcp.h"
#include "command_explain.h"
namespace Ui {
class buying_page;
}

class buying_page : public QDialog
{
    Q_OBJECT

public:
    explicit buying_page(QWidget *parent = nullptr);
    ~buying_page();
private slots:
    void ReadData();
   // void ReadError(QAbstractSocket::SocketError);
    void on_button_ok_clicked();

private:
    Ui::buying_page *ui;
    //QTcpSocket*tcpClient;
    void SolveCommand(QString data);
    TCP*communicate;
    command_explain *explain;
};

#endif // BUYING_PAGE_H
