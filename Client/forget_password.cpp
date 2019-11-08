#include "forget_password.h"
#include "ui_forget_password.h"
#include <QMessageBox>
forget_password::forget_password(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::forget_password)
{
    ui->setupUi(this);

    communicate=new TCP;
    communicate->SetUp();
    connect(communicate->tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
}

forget_password::~forget_password()
{
    delete ui;
}
void forget_password::ReadData()
{
    QByteArray buffer = communicate->tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        SolveCommand(buffer);

    }
}


void forget_password::on_button_getvcode_clicked()//获取验证码
{
    email=new Smtp("453480049@qq.com","dbeoolvblfcrbgif");
    vcode=email->get_randnumber();//获取验证码
    position_email=ui->line_email->text();
    QString touser="您正在进行WeShop的忘记密码操作，本次请求的邮件验证码是：";
    email->send(position_email.toLatin1(),"WeStore",touser+vcode);
}

void forget_password::on_button_ok_clicked()
{
    int flag=check();
    if(!flag)
        return;
    QString factcode=ui->line_vcode->text();
    QString newpassword=ui->line_newpassword->text();

    if(factcode==vcode)//验证码输入正确
    {
        communicate->tcpClient->connectToHost("192.168.182.1", 8888);
        if (communicate->tcpClient->waitForConnected(1000))  // 连接成功则进入if{}
        {
            QString data = "2010#"+newpassword+"#"+position_email+"#";
            communicate->SendDate(data);
        }

    }
}
void forget_password::SolveCommand(QString command)
{
    if(command=="2010")
    {
     QMessageBox msgBox;
     msgBox.setText("密码重置完成！");
     msgBox.exec();
    }
}
int forget_password::check()
{
    QString email=this->ui->line_email->text();
    QString code=this->ui->line_vcode->text();
    QString password=this->ui->line_newpassword->text();

    if(!(email!=""&&code!=""&&password!=""))
    {
        QMessageBox msgBox;
        msgBox.setText("表格未填写完整！");
        msgBox.exec();
        return 0;
    }
    if(password.length()<6)
    {
        QMessageBox msgBox;
        msgBox.setText("密码长度不少于6位！");
        msgBox.exec();
        return 0;
    }
    return 1;
}
