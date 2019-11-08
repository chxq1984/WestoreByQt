#include "addfriend.h"
#include "ui_addfriend.h"

addFriend::addFriend(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addFriend)
{
    ui->setupUi(this);
    communicate=new TCP;
    communicate->SetUp();
    connect(communicate->tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));

}

addFriend::~addFriend()
{
    delete ui;
}
void addFriend::ReadData()
{
    QByteArray buffer =communicate-> tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        SolveCommand(buffer);
    }
}
void addFriend::SolveCommand(QString command)
{
    if(command=="3000")//添加成功
    {
        QMessageBox msgBox;
        msgBox.setText("好友添加成功！");
        msgBox.exec();
    }
    else if(command=="3010")//添加失败
    {
        QMessageBox msgBox;
        msgBox.setText("账号不存在！");
        msgBox.exec();
    }
}
void addFriend::on_button_addfriend_clicked()
{
    communicate->ConnectToHost("127.0.0.1",8888);
    QString account=ui->line_faccount->text();
    QString data="3000#"+account+"#"+global_login_username+"#";
    communicate->SendDate(data);
}
