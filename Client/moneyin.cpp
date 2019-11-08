#include "moneyin.h"
#include "ui_moneyin.h"
#include <QIntValidator>
moneyin::moneyin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::moneyin)
{
    ui->setupUi(this);

    ui->line_money->setValidator(new QIntValidator(1,500000,this));

    communicate=new TCP;
    communicate->SetUp();
    connect(communicate->tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    inmoney=0;
}

moneyin::~moneyin()
{
    delete ui;
}

void moneyin::on_pushButton_clicked()
{
    inmoney=0;
    QString monnum=ui->line_money->text();

    QString password=ui->line_password->text();
    if(!(monnum!=""&&password!=""))
    {
        QMessageBox msgBox;
        msgBox.setText("请将表单填写完整！");
        msgBox.exec();
        return;
    }
    inmoney=monnum.toFloat();
    if(inmoney==0)
    {
        QMessageBox msgBox;
        msgBox.setText("最小金额数为1元！");
        msgBox.exec();
        return;
    }
    if(inmoney+global_balance>500000)
    {
        QMessageBox msgBox;
        msgBox.setText("总金额不能超过五十万元！");
        msgBox.exec();
        return;
    }

    communicate->ConnectToHost("127.0.0.1",8888);
    QString data = "0009#"+monnum+"#"+password+"#"+personcentral_label_account+"#";//发送数据
    communicate->SendDate(data);

}
void moneyin::ReadData()
{
    QByteArray buffer = communicate-> tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        SolveCommand(buffer);

    }
}

void moneyin::SolveCommand(QString command)
{
    if(command=="0009")
    {
        global_balance+=inmoney;
        QMessageBox msgBox;
        msgBox.setText("充值成功！");
        msgBox.exec();
        this->hide();
    }
    else if(command=="0010")
    {
        QMessageBox msgBox;
        msgBox.setText("密码输入错误！");
        msgBox.exec();
    }
}
//购物车还未校验
