#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "command_action.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <qDebug>
#include <QPalette>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpServer = new myserver;
    bool ok = tcpServer->listen(QHostAddress::Any, 8888);
    QPalette pal =this->palette();
    pal.setBrush(QPalette::Background,QBrush(QPixmap("C:/Users/lenovo/Desktop/weather.png")));
    setPalette(pal);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_triggered()
{
    goodsshow=new goods;
    goodsshow->resize(1131,847);
    goodsshow->show();
}
