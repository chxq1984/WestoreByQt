#include "change_name.h"
#include "ui_change_name.h"
#include <QMessageBox>
change_name::change_name(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::change_name)
{
    ui->setupUi(this);
}

change_name::~change_name()
{
    delete ui;
}

void change_name::on_button_ok_clicked()
{
    QString name=ui->line_name->text();

    int found=name.indexOf(' ');
    if(found)
    {
        QMessageBox msgBox;
        msgBox.setText("用户名不能包含空格!");
        msgBox.exec();
        return;
    }

    change_name_name=name;

    communicate=new TCP;
    communicate->SetUp();
    connect(communicate->tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    communicate->ConnectToHost("127.0.0.1",8888);
    QString data = "0006#"+name+"#"+personcentral_label_account+"#";
    communicate->SendDate(data);
}
void change_name::ReadData()
{
    QByteArray buffer = communicate-> tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        SolveCommand(buffer);
    }
}

void change_name::SolveCommand(QString command)
{
            if(command=="0007")//用户名重复
            {
                QMessageBox msgBox;
                msgBox.setText("用户名重复!");
                msgBox.exec();
            }
            else if(command=="0006")//用户名无重复
            {
                QMessageBox msgBox;
                msgBox.setText("用户名设置完成!");
                msgBox.exec();
            }
}
