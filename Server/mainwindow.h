#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QSqlQuery>
#include "command_action.h"
#include "goods.h"
#include "myserver.h"
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_action_triggered();

private:
    Ui::MainWindow *ui;
    myserver *tcpServer;//声明一个服务端
    goods *goodsshow;
};

#endif // MAINWINDOW_H
