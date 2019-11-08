
#include "login.h"
#include "ui_login.h"
#include "registered.h"
#include "globals.h"
#include <QMessageBox>
login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    communicate=new TCP;
    communicate->SetUp();
    connect(communicate->tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

void login::on_pushButton_clicked()
{
    regis=new registered;
    regis->resize(400,400);
    regis->show();
}

void login::on_button_login_clicked()
{
    QString account=ui->line_account->text();
    personcentral_label_account=account;
    QString password=ui->line_password->text();
    global_login_password=password;
    communicate->ConnectToHost("127.0.0.1",8888);
    QString data ="2000#"+account+"#"+password+"#";
    communicate->SendDate(data);
}
void login::ReadData()
{
    QByteArray buffer = communicate->tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        SolveCommand(buffer);
    }
    communicate->DisConnect();
}

void login::on_button_forget_clicked()
{
    forget=new forget_password;
    forget->resize(400,400);
    forget->show();
}
void login::SolveCommand(QString command)
{
    explain=new command_explain;
    explain->decoding(command);
    if(explain->command_set[0]=="0003")
    {
        mainwin=new MainWindow;
        global_login_username=explain->command_set[1];
        global_balance=explain->command_set[2].toFloat();
        this->hide();
        mainwin->resize(1574,886);
        mainwin->show();
    }
    else if(explain->command_set[0]=="0004")
    {
        QMessageBox msgBox;
        msgBox.setText("账号或密码错误！");
        msgBox.exec();
    }

}
