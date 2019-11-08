#include "friendlist.h"
#include "ui_friendlist.h"

friendlist::friendlist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::friendlist)
{
    ui->setupUi(this);
    model=new QStandardItemModel;
    communicate=new TCP;
    communicate->SetUp();
    connect(communicate->tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    communicate->ConnectToHost("127.0.0.1",8888);
    QString data="3006#"+global_login_username+"#";
    communicate->SendDate(data);
    explain=new command_explain;
    set_listhead();
}

friendlist::~friendlist()
{
    delete ui;
}
void friendlist::ReadData()
{
    QByteArray buffer = communicate->tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        SolveCommand(buffer);
    }
}
void friendlist::SolveCommand(QString command)
{
    if(command=="3001"||command=="3011")//3001 3011,删除好友
    {
        if(command=="3001")
        {
            QMessageBox msgBox;
            msgBox.setText("删除成功!");
            msgBox.exec();
            return;
        }
        else if(command=="3011"){
            QMessageBox msgBox;
            msgBox.setText("删除失败!");
            msgBox.exec();
            return;
        }
    }
    if(command=="3006#")
        return;//好友数据为空
    explain->decoding_list(command);
    for(int m=0;m<explain->line_number;m++)
    {
        disPlay(m,explain-> all_line_information.at(m));
    }
}
void friendlist::set_listhead()
{
    model->setHorizontalHeaderItem(0,new QStandardItem("用户名"));
    model->setHorizontalHeaderItem(1,new QStandardItem("账号"));
    model->setHorizontalHeaderItem(2,new QStandardItem("删除"));
    this->ui->table_friend->setModel(model);
}
void friendlist::disPlay(int rol,QString line)
{

    explain->decoding(line);
    for(int m=0;m<explain->command_set.size()-1;m++)
    {
        this->model->setItem(rol,m,new QStandardItem(explain->command_set[m+1]));//行,列
    }
    setlist_button(rol);//添加按钮

}
void friendlist::setlist_button(int row)
{
    QPushButton *button_delete = new QPushButton("删除");
    ui->table_friend->setIndexWidget(model->index(row,2), button_delete);
    connect(button_delete, SIGNAL(clicked()), this, SLOT(deletefriend()));
}

void friendlist::on_button_addfreind_clicked()
{
    show_addfriend =new addFriend;
    show_addfriend->resize(500,500);
    show_addfriend->show();
}
void friendlist::deletefriend()
{
    QMessageBox msgBox;
    msgBox.setText("提示");
    msgBox.setInformativeText("确定删除好友？");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    if(ret==QMessageBox::Yes)//确认删除
    {//3001+好友账号+用户名
        int row=ui->table_friend->currentIndex().row();
        QString friend_account=model->item(row,1)->text();
        QString command="3001#"+friend_account+"#"+global_login_username+"#";
        communicate->SendDate(command);
    }
    else if(ret==QMessageBox::No)
    {
        return;
    }
}
