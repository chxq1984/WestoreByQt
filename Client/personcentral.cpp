#include "personcentral.h"
#include "ui_personcentral.h"
#include<qDebug>
personcentral::personcentral(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::personcentral)
{
    coma=new command_explain;
    ui->setupUi(this);

    communicate=new TCP;
    communicate->SetUp();
    connect(communicate->tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    communicate->ConnectToHost("127.0.0.1",8888);
    QString data = "0008#"+personcentral_label_account+"#";
    communicate->SendDate(data);

}

personcentral::~personcentral()
{
    delete ui;
}

void personcentral::on_button_changename_clicked()
{
    change_names=new change_name;
    this->hide();
    change_names->resize(400,400);
    change_names->show();
}
void personcentral::ReadData()
{
    QByteArray buffer =communicate-> tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        /*coma->decoding(buffer);
        if(coma->command_set[0]=="0008")//设置用户信息
        {
            ui->label_account->setText(personcentral_label_account);
            ui->label_username->setText(coma->command_set[1]);
            ui->label_email->setText(coma->command_set[2]);
            ui->label_balance->setText(coma->command_set[3]);
        }*/
        SolveCommand(buffer);
    }
}


void personcentral::on_button_inmoney_clicked()
{
    money_in=new moneyin;
    money_in->resize(400,400);
    money_in->show();
}
void personcentral::SolveCommand(QString command)
{
    coma->decoding(command);
    if(coma->command_set[0]=="0008")//设置用户信息
    {
        ui->label_account->setText(personcentral_label_account);
        ui->label_username->setText(coma->command_set[1]);
        ui->label_email->setText(coma->command_set[2]);
        ui->label_balance->setText(coma->command_set[3]);
    }
}

void personcentral::on_action_2_triggered()
{
    friendlish_show=new friendlist;
    friendlish_show->resize(487,446);
    friendlish_show->show();
}

void personcentral::on_action_triggered()//收藏夹
{
    showcollect=new colletion;
    showcollect->resize(510,546);
    showcollect->show();
}

void personcentral::on_action_3_triggered()//购物车
{
    showshoppingcar=new shoppingcar;
    showshoppingcar->resize(661,446);
    showshoppingcar->show();
}
