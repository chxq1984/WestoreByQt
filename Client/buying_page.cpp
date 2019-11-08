#include "buying_page.h"
#include "ui_buying_page.h"
#include <QIntValidator>
buying_page::buying_page(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::buying_page)
{
    ui->setupUi(this);

    ui->line_number->setValidator(new QIntValidator(1,200000,this));

    communicate=new TCP;
    communicate->SetUp();
    connect(communicate->tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    ui->lable_goodsname->setText(global_mainwindow_name);
    communicate->ConnectToHost("127.0.0.1", 8888);
    QString data="1111#"+global_mainwindow_name+"#";
    communicate->SendDate(data);
}

buying_page::~buying_page()
{
    delete ui;
}

void buying_page::ReadData()
{
    QByteArray buffer =communicate-> tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        SolveCommand(buffer);
    }
}

void buying_page::on_button_ok_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("支付提醒");
    msgBox.setInformativeText("确认支付");
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    if(ret==QMessageBox::No)
    {
        return;
    }
    QString good_number=ui->line_number->text();
    QString password=ui->line_password->text();
    int goodsnumberCheck=good_number.toInt();
    if(goodsnumberCheck==0)
    {
        QMessageBox msgBox;
        msgBox.setText("购买数量至少为1件!");
        msgBox.exec();
        return;
    }

    QString data ="1001#"+global_mainwindow_name+"#";
    data+=good_number+"#"+password+"#"+personcentral_label_account+"#";
    communicate->SendDate(data);
}
void buying_page::SolveCommand(QString data)
{
            if(data=="1001")//支付成功
            {
                QMessageBox msgBox;
                msgBox.setText("支付成功!");
                msgBox.exec();
                this->hide();
            }
            else if(data=="1011")
            {
                QMessageBox msgBox;
                msgBox.setText("购买失败!");
                msgBox.exec();
            }
            else {
                //获取优惠信息 1111+标志1+标志2+折扣一+折扣二1+折扣二2+折扣二3
                explain=new command_explain;
                explain->decoding(data);
                QString flag_total=explain->command_set[1];
                QString flag_add=explain->command_set[2];
                if(flag_total!="0"||flag_add!="0")
                {
                    QString data;
                    if(flag_total!="0")
                    {
                        QString discounttotal=explain->command_set[3];
                        data+="此商品享受统一"+discounttotal+"折优惠\n";
                        if(flag_add!="0")
                            data+="同时";
                    }
                    if(flag_add!="0")
                    {
                        QString disAddOne=explain->command_set[4];
                        QString disAddTwo=explain->command_set[5];
                        QString disAddThree=explain->command_set[6];
                        qDebug()<<disAddOne<<" "<<disAddTwo<<" "<<disAddThree;
                        data+="此商品享受叠加优惠\n";
                        if(disAddOne!="1")
                        {
                            data+="第一件"+disAddOne+"折\n";
                        }
                        if(disAddTwo!="1")
                        {
                            data+="第二件"+disAddTwo+"折\n";
                        }
                        if(disAddThree!="1")
                        {
                            data+="第三件"+disAddThree+"折\n";
                        }
                    }
                    QMessageBox msgBox;
                    msgBox.setText(data);
                    msgBox.exec();
                }
            }
}
