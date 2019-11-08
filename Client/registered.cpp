#include "registered.h"
#include "ui_registered.h"
#include "smtp.h"
#include <QString>
#include <QtGlobal>
#include <QTime>
#include <QMessageBox>
#include<qDebug>
registered::registered(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registered)
{
    ui->setupUi(this);
}

registered::~registered()
{
    delete ui;
}

void registered::on_button_getcode_clicked()
{

    email =new Smtp("453480049@qq.com","dbeoolvblfcrbgif");
    position_email=ui->line_email->text();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    QString map="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for(int m=0;m<6;m++)
    {
        int random=qrand()%map.length();
        vcode+=map[random];
    }
    QString touser="您正在进行WeShop的账号注册操作，本次请求的邮件验证码是：";
    email->send(position_email.toLatin1(),"WeShop",touser+vcode);
    qDebug()<<vcode;
}

void registered::on_button_ok_clicked()
{
    int flag = checkText();
    if(!flag)
        return;
    QString getvcode=ui->line_code->text();//用户输入的验证码
    if(getvcode!=vcode)
    {
        QMessageBox msgBox;
        msgBox.setText("验证码错误!");
        msgBox.exec();
        return;
    }
    communicate=new TCP;
    communicate->SetUp();
    connect(communicate->tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    communicate->ConnectToHost("127.0.0.1",8888);
    QString data ="2001#";//先发送账号进行查重
    account=ui->line_account->text();

    password=ui->line_password->text();
    data=data+account+"#";
    if(data != "")
    {
        communicate->SendDate(data);
    }
    /*tcpClient = new QTcpSocket(this);   //实例化tcpClient
    tcpClient->abort();                 //取消原有连接
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), \
                this, SLOT(ReadError(QAbstractSocket::SocketError)));
    tcpClient->connectToHost("192.168.182.1", 8888);
    if (tcpClient->waitForConnected(1000))  // 连接成功则进入if{}
    {
        qDebug()<<"成功连接服务器！";
    }
    QString data ="2001#";//先发送账号进行查重
    account=ui->line_account->text();
    password=ui->line_password->text();
    data=data+account+"#";
    if(data != "")
    {
        bool a=tcpClient->write(data.toLatin1()); //QString转QByteArray
        /*if(a)
            qDebug()<<data;*/
    //}
    /*else
    {
        //处理验证码错误
    }*/



}
void registered::ReadData()
{
    QByteArray buffer = communicate-> tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        //处理buffer内从服务端得到的数据
        if(buffer=="0000")//账号已存在
        {
            QMessageBox msgBox;
            msgBox.setText("此账号已被注册！");
            msgBox.exec();
        }
        else if(buffer=="0001")//账号可注册
        {
            QString data = "2002#"+password+"#"+position_email+"#";
            communicate->SendDate(data);
        }
        else if(buffer=="0002")//登记完成
        {
            QMessageBox msgBox;
            msgBox.setText("注册成功！");
            msgBox.exec();
        }
    }
}
int registered::checkText()//输入校验
{
    QString account=ui->line_account->text();
    QString password=ui->line_password->text();
    QString email=ui->line_email->text();
    QString code=ui->line_code->text();

    if(!(account!=""&&password!=""&&email!=""&&code!=""))
    {
        QMessageBox msgBox;
        msgBox.setText("信息未填写完整！");
        msgBox.exec();
        return 0;
    }
    for(int i=0;i<account.length();i++)
    {
        if(!(account[i]>='0'&&account[i]<='9'))
        {
            QMessageBox msgBox;
            msgBox.setText("账号只允许输入数字！");
            msgBox.exec();
            return 0;
        }
    }
    if(account.length()<7)
    {
        QMessageBox msgBox;
        msgBox.setText("账号长度不可少于7位！");
        msgBox.exec();
        return 0;
    }
    for(int i=0;i<password.length();i++)
    {
        if(!((password[i]>='0'&&password[i]<='9')||(password[i]>='a'&&password[i]<='z')||(password[i]>='A'&&password[i]<='Z')))
        {
            QMessageBox msgBox;
            msgBox.setText("密码只允许输入数字与英文字符！");
            msgBox.exec();
            return 0;
        }
    }
    if(password.length()<6)
    {
        QMessageBox msgBox;
        msgBox.setText("密码长度不可少于6位！");
        msgBox.exec();
        return 0;
    }
    return 1;
}
